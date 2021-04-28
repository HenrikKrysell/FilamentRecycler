import signal
from FilamentRecyclerController.controller import Controller
from FrontendCommunication.FrontendComServer import FrontendComServer
from pymitter import EventEmitter


# Gracefully close the app when docker closes
def handle_sigterm(*args):
    raise KeyboardInterrupt()
signal.signal(signal.SIGTERM, handle_sigterm)

if __name__ == '__main__':
    eventEmitter = EventEmitter()

    controller = Controller(eventEmitter)
    controller.start()

    frontendComServer = FrontendComServer(eventEmitter)
    frontendComServer.init(5001)
