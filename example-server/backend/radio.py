import logging as log
from struct import unpack
from binascii import hexlify
from time import sleep, time
from threading import Thread
from tinybrd import Radio
from .events import Dispatcher


class RadioFlags:
    """
    Flags and status codes used in our protocol.
    """

    #Flags
    ENABLE_BMP180 = 1   #BMP180 enabled flag
    ENABLE_TSL2561 = 2  #TSL2561 enabled flag
    ENABLE_HTU21 = 4    #HTU21 enabled flag

    #Status codes

    #BMP180
    BMP180_ERR_INIT = 1          #initialization error
    BMP180_ERR_TEMP_START = 2    #temperature measurement request error
    BMP180_ERR_TEMP_READ = 4     #temperature measurement read error
    BMP180_ERR_PRESS_START = 8   #presure measurement request error
    BMP180_ERR_PRESS_READ = 16   #presure measurement read error
    BMP180_ERR = BMP180_ERR_INIT | BMP180_ERR_TEMP_START | BMP180_ERR_TEMP_READ | BMP180_ERR_PRESS_START | BMP180_ERR_PRESS_READ

    #TSL2561
    TSL2561_ERR_READ = 32   #measurement read error
    TSL2561_ERR_RANGE = 64  #measurement out of range error
    TSL2561_ERR = TSL2561_ERR_READ | TSL2561_ERR_RANGE

    #HTU21
    HTU21_ERR = 128 #generic error


class RadioThread(Thread):
    """
    Radio communication handling thread.
    """
    running = True
    last_message = 0

    def __init__(self, name, dispatcher: Dispatcher, channel, address, daemon=None):
        super().__init__(name=name, daemon=daemon)

        self.dispatcher = dispatcher
        self.channel = channel
        self.address = bytes(address)
        self.radio = Radio(self.address, self.channel)

    def reset_radio(self):
        """
        Reset radio procedure.
        """
        log.warning("Resetting radio")

        # cleanup library internals to peaceful reset
        self.radio.off()
        self.radio.device._protocol.spi.close()
        del self.radio

        self.radio = Radio(self.address, self.channel)
        self.last_message = time()

    def stop(self):
        self.running = False

    def run(self):
        self.last_message = time()

        while self.running:
            try:

                # sometimes communication between rPI and NRF24L01P fails.
                # this is the simplest way to detect the condition and reset the radio.
                if time() - self.last_message > 60:
                    self.reset_radio()

                if self.radio.available():
                    data = self.radio.read()
                    if len(data) == 27:
                        self.last_message = time()
                        self.process_message(data)

                    else:
                        log.warning("Got invalid message: {} length {}".format(hexlify(data), len(data)))
                else:
                    sleep(0.5)

            except Exception as e:
                log.exception(str(e))

    def process_message(self, data):
        """
        Decode the packet, scale some readings, clear readings of not available sensors
        and notify about new data through event dispatcher.
        """
        record = {'stamp': int(time())}

        record['sensor_id'], \
            record['seq'], \
            record['flags'], \
            record['status'], \
            record['bar_temp'], \
            record['bar_pres_abs'], \
            record['bar_pres_rel'], \
            record['lux'], \
            record['hum_temp'], \
            record['hum_hum'], \
            record['vbat'], \
            record['vreg'] \
            = unpack('=BHBBhfffhHHH', data)


        record['bar_temp'] /= 100
        record['hum_temp'] /= 100
        record['hum_hum'] /= 100
        record['vbat'] /= 100
        record['vreg'] /= 100

        if not (record['flags'] & RadioFlags.ENABLE_BMP180) or (record['status'] & RadioFlags.BMP180_ERR):
            record['bar_temp'] = None
            record['bar_pres_abs'] = None
            record['bar_pres_rel'] = None

        if not (record['flags'] & RadioFlags.ENABLE_HTU21) or (record['status'] & RadioFlags.HTU21_ERR):
            record['hum_temp'] = None
            record['hum_hum'] = None

        if not (record['flags'] & RadioFlags.ENABLE_TSL2561) or (record['status'] & RadioFlags.TSL2561_ERR):
            record['lux'] = None

        self.dispatcher.trigger('new_reading', self, record=record)


def start_radio(dispatcher: Dispatcher, channel = 100, address = (1, 0, 0)):
    radio = RadioThread(name='radio', dispatcher=dispatcher, channel=channel, address=address, daemon=True)
    radio.start()
