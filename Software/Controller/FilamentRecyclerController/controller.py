from time import sleep
import time
import signal
import asyncio
from typing import cast
from simple_pid import PID

import os,sys
from Microcontroller.Messages.FromMicrocontroller.InitializationCompleteMessage import InitializationCompleteMessage
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants
from Microcontroller.Messages.FromMicrocontroller.WelcomeMessage import WelcomeMessage
from Microcontroller.Messages.FromMicrocontroller.ErrorMessage import ErrorMessage
from Microcontroller.Messages.FromMicrocontroller.TelemetricDataMessage import TelemetricDataMessage
from Microcontroller.Messages.ToMicrocontroller.InitializeMessage import InitializeMessage
from Microcontroller.Messages.ToMicrocontroller.SetValuesMessage import SetValuesMessage
from .RPMCounter import RPMCounter


class Controller:
  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

    self.eventEmitter.on(constants.FRONTEND_BASE_MESSAGE+'setRPM', self.__setRPMHandler)
    self.eventEmitter.on(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, self._onRecievedMessageFromMicrocontroller)
    self.microcontrollerInitialized = False
    self._rpmCounter = None
    self._augerMotorPID = None
    self._latestAugerSpeed = 0

  def __setRPMHandler(self, args):
    print("setRPM: {args}".format(args = args))
    if (self._augerMotorPID):
      self._augerMotorPID.setpoint = args['targetRPM']

  def _onRecievedMessageFromMicrocontroller(self, msg):
    #print("incomming message: {m}".format(m = msg))
    if (isinstance(msg, WelcomeMessage)):
      initializeMessage = InitializeMessage()
      initializeMessage.motorINAPin = 14
      initializeMessage.motorINBPin = 18
      initializeMessage.motorPWMPin = 3
      initializeMessage.motorDir = 1

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
      initializeMessage.filamentDiameterAnalogPin = 67
      initializeMessage.filamentGuideEndstopPin = 19

      initializeMessage.telemetricDataIntervalMs = 500
      initializeMessage.thermistorPollIntervalMs = 500
      initializeMessage.winderArmPollIntervalMs = 500
      initializeMessage.filamentDiameterPollIntervalMs = 500
      initializeMessage.motorRPMCounterPollIntervalMs = 40
      
      self._rpmCounter = RPMCounter(10,initializeMessage.telemetricDataIntervalMs,140)
      
      self._augerMotorPID = PID(Kp=40, Ki=30, Kd=0, setpoint=0.5)
      self._augerMotorPID.sample_time = (initializeMessage.telemetricDataIntervalMs - 100) / 1000

      self.eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, initializeMessage)

    elif (isinstance(msg, InitializationCompleteMessage)):
      print("Controller::{m}".format(m = msg))
      self.microcontrollerInitialized = True
    elif (isinstance(msg, TelemetricDataMessage)):
      m: TelemetricDataMessage = msg
      self._rpmCounter.addSample(m.rpmCount)

      t = self._augerMotorPID(self._rpmCounter.currentRPM)
      self._latestAugerSpeed = max(min(255, t), 0)
      
      self.eventEmitter.emit(constants.CONTROLLER_STATE_MESSAGE, {"currentRPM": self._rpmCounter.currentRPM, "targetRPM": self._augerMotorPID.setpoint})

  async def __run(self):
    lastTime = time.time()
    augerSpeed = 50
    variableStepper = 2
    await asyncio.sleep(1)
    while self.isRunning:
      await asyncio.sleep(0.001)
      if (time.time() - lastTime > 0.5):
        values = SetValuesMessage()
        values.augerMotorSpeed = self._latestAugerSpeed # 127 #augerSpeed # min 40 without load
        values.winderStepperRPM = 0
        values.pullerStepperRPM = 0
        values.filamentGuideSteps = 0
        self.eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, values)
        augerSpeed += 10
        augerSpeed = 50 if augerSpeed > 255 else augerSpeed
        variableStepper += 10
        variableStepper =  2 if variableStepper > 255 else variableStepper
        lastTime = time.time()

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
