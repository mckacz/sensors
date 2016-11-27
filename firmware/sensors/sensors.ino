#include <Wire.h>
#include <SPI.h>

#include "sensors.h"
#include "barometer.h"
#include "light.h"
#include "htu.h"
#include "vcc.h"
#include "ui.h"
#include "lib/power.h"
#include "lib/radio.h"

options_t options;

Vcc vcc;
readings_t readings;

void vcc_read();
void reset_readings();

void setup() {
    //power up sensors
    pinMode(PIN_POWER, OUTPUT);
    digitalWrite(PIN_POWER, HIGH);

    //read or setup options
    ui_setup(options);

    //initialize enabled sensors
    if(options.flags & ENABLE_BMP180) {
        barometer_begin();
    }

    if(options.flags & ENABLE_TSL2561) {
        light_begin();
    }

    if(options.flags & ENABLE_HTU21) {
        htu_begin();
    }

    //set common reading values
    readings.id = options.id;
    readings.seq = 0;
    readings.flags = options.flags;
}

void loop() {
    reset_readings();

    //power up sensors and radio
    digitalWrite(PIN_POWER, HIGH);
    digitalWrite(PIN_RADIO_CSN, HIGH);

    //wait for power to settle
    delay(500);

    //read voltages
    vcc_read();

    //read values from enabled sensors
    if(options.flags & ENABLE_BMP180) {
        barometer_read(readings, options);
    }

    if(options.flags & ENABLE_TSL2561) {
        light_read(readings);
    }

    if(options.flags & ENABLE_HTU21) {
        htu_read(readings);
    }

    //send whose struct through radio
    Radio.begin(options.sen_addr, options.channel, PIN_RADIO_CSN, PIN_RADIO_CE);
    Radio.write(options.hub_addr, readings);
    Radio.flush();

    //power off radio (software)
    Radio.off();
    SPI.end();

    //manually reset SPI and radio pins to input
    //without this, radio will drain battery and hang Arduino after switching power off in next step.
    pinMode(MOSI, INPUT);
    pinMode(MISO, INPUT);
    pinMode(SCK, INPUT);
    pinMode(PIN_RADIO_CE, INPUT);
    pinMode(PIN_RADIO_CSN, INPUT);

    //power off sensors and radio
    digitalWrite(PIN_POWER, LOW);

    //print reading if diagnostinc mode is enabled
    ui_loop(readings);

    //increment sequence number
    readings.seq++;

    //deep sleep
    sleep(1000 * options.interval);
}

void reset_readings() {
    readings.status = 0;
    readings.bar_temp = 0;
    readings.bar_pres_abs = 0;
    readings.bar_pres_rel = 0;
    readings.htu_temp = 0;
    readings.htu_hum = 0;
    readings.vbat = 0;
    readings.vreg = 0;
}

void vcc_read() {
    readings.vbat = vcc.Read_Volts() * 100.0 * options.vbat_correct;
    readings.vreg = (analogRead(A0) / 1024.0) * readings.vbat * options.vreg_correct;
}