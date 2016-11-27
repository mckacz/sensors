#!/usr/bin/env python3
import logging as log
from backend.events import Dispatcher
from backend.ui import start_ui
from backend.radio import start_radio

# Config
channel=100
address=(1, 0, 0)
port=8000
debug=True

sensors={
    '1': 'Balcony',
    '2': 'Living room',
    '3': 'Bedroom',
    '4': 'Office',
    '5': 'Bathroom',
}

# Setup logging
log.basicConfig(
    level=log.DEBUG,
    format='%(asctime)s %(threadName)s %(levelname)s: %(message)s',
    filename=__file__ + '.log'
)

console = log.StreamHandler()
console.setFormatter(log.Formatter('%(asctime)s %(threadName)s %(levelname)s: %(message)s'))
log.getLogger('').addHandler(console)

# Start radio and UI
dispatcher = Dispatcher()
start_radio(dispatcher, channel, address)
start_ui(dispatcher, sensors, port, debug)