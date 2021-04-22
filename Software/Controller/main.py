import signal
from Webserver.webserver import Webserver
from FilamentRecyclerController.controller import Controller

# Gracefully close the app when docker closes
def handle_sigterm(*args):
    raise KeyboardInterrupt()
signal.signal(signal.SIGTERM, handle_sigterm)

if __name__ == '__main__':
    print("main")
    controller = Controller()
    controller.start()

    webserver = Webserver()    
    webserver.run(controller)