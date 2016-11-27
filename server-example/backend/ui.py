import json
from os.path import dirname, realpath
from tornado.ioloop import IOLoop
from tornado.web import Application, StaticFileHandler
from tornado.websocket import WebSocketHandler
from .events import Dispatcher


class WSHandler(WebSocketHandler):
    """
    WebSocket connection handler
    """

    def initialize(self, sensors = None, readings = None, clients = None):
        """
        Store references to sensors description, last readings and client list.
        """

        self.sensors = sensors
        self.readings = readings
        self.clients = clients

    def open(self, *args, **kwargs):
        """
        On new connection, append it to clients list and send "hello" packet
        with sensors description and last readings.
        """

        self.clients.append(self)

        hello = {
            'sensors': self.sensors,
            'readings': self.readings
        }

        self.write_message(json.dumps(hello))

    def on_close(self):
        """
        On connection close, remove it from clients list.
        """
        self.clients.remove(self)


def start_ui(dispatcher: Dispatcher, sensors, port=8000, debug=False):
    """
      Start UI by:
      - initialize empty dict for last readings and empty list for clients
      - configure Tornado application
      - create "new_reading" event handler that will sent message to connected clients
        each time a new reading will be available
      - start Tornado IO loop
    """
    res_dir = dirname(dirname(realpath(__file__))) + '/static'

    readings = {}
    clients = []

    app = Application(
        [
            ('/ws', WSHandler, {'sensors': sensors, 'readings': readings, 'clients': clients}),
            ('/(.*)', StaticFileHandler, {'path': res_dir, 'default_filename': 'index.html'})
        ],
        debug=debug
    )

    def handler(event, sender, record):
        readings[record['sensor_id']] = record
        msg = json.dumps({'readings': {record['sensor_id']: record}})

        for client in clients:
            try:
                client.write_message(msg)
            except:
                pass

    dispatcher.on('new_reading', handler)

    app.listen(port)
    IOLoop.instance().start()
