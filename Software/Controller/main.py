import signal
from FilamentRecyclerController.controller import Controller
from FrontendCommunication.FrontendComServer import FrontendComServer
from pymitter import EventEmitter
from Arduino.SerialConnection import SerialConnection
from time import sleep
import constants

# Gracefully close the app when docker closes
def handle_sigterm(*args):
    raise KeyboardInterrupt()
signal.signal(signal.SIGTERM, handle_sigterm)

if __name__ == '__main__':
    eventEmitter = EventEmitter()

    controller = Controller(eventEmitter)
    controller.start()

    serialConnection = SerialConnection(eventEmitter)
    serialConnection.start()

    frontendComServer = FrontendComServer(eventEmitter)
    frontendComServer.init(5001)

    sleep(3)
    eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, "T C P\n")
    eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, "S E500\n")
