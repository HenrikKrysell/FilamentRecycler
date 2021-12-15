from time import sleep
import time
import signal
import asyncio

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants

class Controller:
  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

    self.eventEmitter.on(constants.FRONTEND_BASE_MESSAGE+'test', self.__testHandler)

  def __testHandler(self, args):
    print("incomming message: {args}".format(args = args))

  async def __run(self):
    lastTime = time.time()
    await asyncio.sleep(1)
    while self.isRunning:
      await asyncio.sleep(0.001)
      if (time.time() - lastTime > 10):
        self.eventEmitter.emit(constants.CONTROLLER_STATE_MESSAGE, {"time": lastTime})
        lastTime = time.time()

    print("Closing")

  def stop(self):
    print("Controller: Stopping...")
    self.isRunning = False

  async def start(self):
    print("Controller: Starting...")

    self.isRunning = True
    await self.__run()
