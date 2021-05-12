import threading
from time import sleep
import serial
import serial.tools.list_ports
import time
import signal

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants

class Controller:
  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

    self.eventEmitter.on(constants.FRONTEND_BASE_MESSAGE+'test', self.__testHandler)

  def __testHandler(self, args):
    print("incomming message: {args}".format(args = args))
    self.eventEmitter.emit(constants.MICROCONTROLLER_MESSAGE, {"message": "HELLO FRONTEND!"})

  def getAllComports(self):
    return [comport.device for comport in serial.tools.list_ports.comports()]

  def __run(self):
    print([comport.device for comport in serial.tools.list_ports.comports()])

    lastTime = time.time()
    while self.isRunning:
      sleep(0.001)
      if (time.time() - lastTime > 1):
        #print("controller...." + str(threading.get_ident()))
        self.eventEmitter.emit(constants.CONTROLLER_STATE_MESSAGE, {"time": lastTime})
        lastTime = time.time()

    print("Closing")

  def stop(self):
    print("Stopping controller...")
    self.isRunning = False
    self.thread.stop()

  def start(self):
    print("Starting controller...")
    signal.signal(signal.SIGINT, lambda *args: self.stop())
    signal.signal(signal.SIGTERM, lambda *args: self.stop())

    self.isRunning = True
    self.thread = threading.Thread(target=self.__run)
    self.thread.start()
