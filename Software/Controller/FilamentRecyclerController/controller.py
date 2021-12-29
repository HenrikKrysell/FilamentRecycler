from time import sleep
import time
import signal
import asyncio
from typing import cast

import os,sys
from Microcontroller.Messages.FromMicrocontroller.InitializationCompleteMessage import InitializationCompleteMessage
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants
from Microcontroller.Messages.FromMicrocontroller.WelcomeMessage import WelcomeMessage
from Microcontroller.Messages.FromMicrocontroller.ErrorMessage import ErrorMessage
from Microcontroller.Messages.ToMicrocontroller.InitializeMessage import InitializeMessage
from Microcontroller.Messages.ToMicrocontroller.SetValuesMessage import SetValuesMessage


class Controller:
  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

    self.eventEmitter.on(constants.FRONTEND_BASE_MESSAGE+'test', self.__testHandler)
    self.eventEmitter.on(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, self._onRecievedMessageFromMicrocontroller)
    self.microcontrollerInitialized = False

  def __testHandler(self, args):
    print("incomming frontend message: {args}".format(args = args))

  def _onRecievedMessageFromMicrocontroller(self, msg):
    print("incomming message: {m}".format(m = msg))
    if (isinstance(msg, WelcomeMessage)):
      initializeMessage = InitializeMessage()
      initializeMessage.motorINAPin = 14
      initializeMessage.motorINBPin = 18
      initializeMessage.motorPWMPin = 3

      initializeMessage.motorRPMCounterPin = 2

      initializeMessage.pullerStepperDirPin = 61
      initializeMessage.pullerStepperDirValue = 1
      initializeMessage.pullerStepperEnablePin = 56
      initializeMessage.pullerStepperStepPin = 60

      initializeMessage.winderStepperDirPin = 48
      initializeMessage.winderStepperDirValue = 1
      initializeMessage.winderStepperEnablePin = 62
      initializeMessage.winderStepperStepPin = 46

      initializeMessage.filamentGuideStepperDirPin = 55
      initializeMessage.filamentGuideStepperDirValue = 1
      initializeMessage.filamentGuideStepperEnablePin = 38
      initializeMessage.filamentGuideStepperStepPin = 54

      initializeMessage.thermistorAnalogPin = 69
      initializeMessage.winderArmAnalogPin = 68
      
      self.eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, initializeMessage)
    elif (isinstance(msg, InitializationCompleteMessage)):
      print("Controller::{m}".format(m = msg))
      self.microcontrollerInitialized = True

  async def __run(self):
    lastTime = time.time()
    augerSpeed = 50
    pullerRPM = 2
    await asyncio.sleep(1)
    while self.isRunning:
      await asyncio.sleep(0.001)
      if (time.time() - lastTime > 0.5):
        values = SetValuesMessage()
        values.augerMotorSpeed = augerSpeed
        values.winderStepperRPM = 255
        values.pullerStepperRPM = pullerRPM
        values.filamentGuideSteps = 0
        self.eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, values)
        augerSpeed += 50
        augerSpeed = 50 if augerSpeed > 255 else augerSpeed
        pullerRPM += 50
        pullerRPM =  2 if pullerRPM > 255 else pullerRPM
        lastTime = time.time()
        print('Sending...')

      #if (time.time() - lastTime > 10):
        # self.eventEmitter.emit(constants.CONTROLLER_STATE_MESSAGE, {"time": lastTime})
        # lastTime = time.time()

    print("Closing")

  def stop(self):
    print("Controller: Stopping...")
    self.isRunning = False

  async def start(self):
    print("Controller: Starting...")

    self.isRunning = True
    await self.__run()
