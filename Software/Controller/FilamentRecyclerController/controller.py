import threading
from time import sleep
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

  def __run(self):
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
