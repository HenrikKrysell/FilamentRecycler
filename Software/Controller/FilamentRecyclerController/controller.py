from enum import IntEnum
from xmlrpc.client import boolean

from .PIDTuner import PIDTuner
from .RPMCounter import RPMCounter
from .Thermistor import Thermistor
from Microcontroller.Messages.ToMicrocontroller.SetValuesMessage import SetValuesMessage
from Microcontroller.Messages.ToMicrocontroller.InitializeMessage import InitializeMessage
from Microcontroller.Messages.FromMicrocontroller.TelemetricDataMessage import TelemetricDataMessage
from Microcontroller.Messages.FromMicrocontroller.ErrorMessage import ErrorMessage
from Microcontroller.Messages.FromMicrocontroller.WelcomeMessage import WelcomeMessage
import constants
from time import sleep
import time
import signal
import asyncio
from typing import cast
from simple_pid import PID

import os
import sys
from Microcontroller.Messages.FromMicrocontroller.InitializationCompleteMessage import InitializationCompleteMessage
sys.path.insert(1, os.path.join(sys.path[0], '..'))


class CONTROLLER_STATE(IntEnum):
    STOPPED = 0
    HEATER_PID_TUNE = 1
    PRODUCTION = 2


class Controller:
    def __init__(self, eventEmitter):
        self._eventEmitter = eventEmitter
        self._state = CONTROLLER_STATE.STOPPED
        self._lastSetNewValuesTime = 0
        self._setNewValuesIntervalSec = 0.5

        self._eventEmitter.on(
            constants.FRONTEND_BASE_MESSAGE+'setRPM', self.__setRPMHandler)
        self._eventEmitter.on(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER,
                              self._onRecievedMessageFromMicrocontroller)
        self._microcontrollerInitialized = False
        self._rpmCounter = None
        self._augerMotorPID = None
        self._latestAugerSpeed = 0
        self._thermistor = None
        self._heaterPIDTuner = None

    def __setRPMHandler(self, args):
        print("setRPM: {args}".format(args=args))
        if (self._augerMotorPID):
            self._augerMotorPID.setpoint = args['targetRPM']

    def __sendInitializationMessage(self):
        # Read this data from the database instead!
        # --r0=100000 --t0=25 --r1=0 --r2=4700 --beta=4066 --max-adc=1023
        # --beta=4092
        # --beta=3960
        self._thermistor = Thermistor(
            r0=100000, t0=25, beta=4092, r1=0, r2=4700, numSamples=2)

        initializeMessage = InitializeMessage()

        initializeMessage.heaterPin = 8
        initializeMessage.filamentGuideEndstopPin = 19
        initializeMessage.telemetricDataIntervalMs = 500

        initializeMessage.motorDriver.inAPin = 14
        initializeMessage.motorDriver.inBPin = 18
        initializeMessage.motorDriver.pwmPin = 3
        initializeMessage.motorDriver.dir = 1

        initializeMessage.motorRPMCounter.pin = 2
        initializeMessage.motorRPMCounter.pollIntervalMs = 40
        initializeMessage.motorRPMCounter.delayBetweenFirstPoll = 100

        initializeMessage.pullerStepper.dirPin = 61
        initializeMessage.pullerStepper.dirValue = 1
        initializeMessage.pullerStepper.enablePin = 56
        initializeMessage.pullerStepper.stepPin = 60

        initializeMessage.winderStepper.dirPin = 48
        initializeMessage.winderStepper.dirValue = 1
        initializeMessage.winderStepper.enablePin = 62
        initializeMessage.winderStepper.stepPin = 46

        initializeMessage.filamentGuideStepper.dirPin = 55
        initializeMessage.filamentGuideStepper.dirValue = 1
        initializeMessage.filamentGuideStepper.enablePin = 38
        initializeMessage.filamentGuideStepper.stepPin = 54

        initializeMessage.thermistorConfig.pin = 68
        initializeMessage.thermistorConfig.pollIntervalMs = 500
        initializeMessage.thermistorConfig.delayBetweenFirstPoll = 500
        initializeMessage.thermistorConfig.numSamples = 8
        initializeMessage.thermistorConfig.delayBetweenSamples = 1

        initializeMessage.winderArmConfig.pin = 69
        initializeMessage.winderArmConfig.pollIntervalMs = 500
        initializeMessage.winderArmConfig.delayBetweenFirstPoll = 500
        initializeMessage.winderArmConfig.numSamples = 1
        initializeMessage.winderArmConfig.delayBetweenSamples = 1

        initializeMessage.filamentDiamterConfig.pin = 67
        initializeMessage.filamentDiamterConfig.pollIntervalMs = 500
        initializeMessage.filamentDiamterConfig.delayBetweenFirstPoll = 500
        initializeMessage.filamentDiamterConfig.numSamples = 1
        initializeMessage.filamentDiamterConfig.delayBetweenSamples = 1

        self._rpmCounter = RPMCounter(
            10, initializeMessage.telemetricDataIntervalMs, 140)

        self._augerMotorPID = PID(Kp=40, Ki=30, Kd=0, setpoint=0.5)
        self._augerMotorPID.sample_time = (
            initializeMessage.telemetricDataIntervalMs - 100) / 1000

        self._eventEmitter.emit(
            constants.SEND_MESSAGE_TO_MICROCONTROLLER, initializeMessage)

    def __telemetricDataHandler(self, m: TelemetricDataMessage):
        self._rpmCounter.addSample(m.rpmCount)
        self._thermistor.addSample(m.temperature)

        print("Temperature: {t}".format(t=self._thermistor.temperatureC))

        t = self._augerMotorPID(self._rpmCounter.currentRPM)
        self._latestAugerSpeed = max(min(255, t), 0)

        if (self._heaterPIDTuner != None):
            self._heaterPIDTuner.temperature_update(
                time.time(), self._thermistor.temperatureC)

        self._eventEmitter.emit(constants.CONTROLLER_STATE_MESSAGE, {
                                "currentRPM": self._rpmCounter.currentRPM,
                                "targetRPM": self._augerMotorPID.setpoint,
                                "temperature": self._thermistor.temperatureC,
                                "state": self._state,
                                })

    def _onRecievedMessageFromMicrocontroller(self, msg):
        #print("incomming message: {m}".format(m = msg))
        if (isinstance(msg, WelcomeMessage)):
            self.__sendInitializationMessage()
        elif (isinstance(msg, InitializationCompleteMessage)):
            print("Controller::{m}".format(m=msg))
            self._microcontrollerInitialized = True
            # DEBUG
            self._onChangeState(CONTROLLER_STATE.HEATER_PID_TUNE)
        elif (isinstance(msg, TelemetricDataMessage)):
            self.__telemetricDataHandler(msg)

    def _setNewValues(self, augerMotorSpeed: int, winderStepperRPM: int, pullerStepperRPM: int, filamentGuideSteps: int, emergencyStop: boolean, turnOnHeater: boolean):
        if (time.time() - self._lastSetNewValuesTime > self._setNewValuesIntervalSec):
            values = SetValuesMessage()
            values.emergencyStop = emergencyStop
            values.turnOnHeater = turnOnHeater
            values.augerMotorSpeed = augerMotorSpeed
            values.winderStepperRPM = winderStepperRPM
            values.pullerStepperRPM = pullerStepperRPM
            values.filamentGuideSteps = filamentGuideSteps
            self._eventEmitter.emit(
                constants.SEND_MESSAGE_TO_MICROCONTROLLER, values)
            self._lastSetNewValuesTime = time.time()

    def _onChangeState(self, newState: CONTROLLER_STATE):
        if (newState == CONTROLLER_STATE.STOPPED):
            self._state = newState
        elif (newState == CONTROLLER_STATE.PRODUCTION):
            self._state = newState
        elif (newState == CONTROLLER_STATE.HEATER_PID_TUNE):
            self._state = newState
            self._heaterPIDTuner = PIDTuner(170)

    async def __run(self):
        await asyncio.sleep(1)
        while self.isRunning:
            await asyncio.sleep(0.001)
            if (self._state == CONTROLLER_STATE.STOPPED):
                self._setNewValues(
                    augerMotorSpeed=0,
                    winderStepperRPM=0,
                    pullerStepperRPM=0,
                    filamentGuideSteps=0,
                    emergencyStop=False,
                    turnOnHeater=False
                )
            elif (self._state == CONTROLLER_STATE.PRODUCTION):
                self._setNewValues(
                    augerMotorSpeed=self._latestAugerSpeed,
                    winderStepperRPM=255,
                    pullerStepperRPM=255,
                    filamentGuideSteps=0,
                    emergencyStop=False,
                    turnOnHeater=False
                )
            elif (self._state == CONTROLLER_STATE.HEATER_PID_TUNE):
                self._setNewValues(
                    augerMotorSpeed=0,
                    winderStepperRPM=0,
                    pullerStepperRPM=0,
                    filamentGuideSteps=0,
                    emergencyStop=False,
                    turnOnHeater=self._heaterPIDTuner.Heating
                )
                if (not self._heaterPIDTuner.check_busy):
                    self._onChangeState(CONTROLLER_STATE.STOPPED)
                    self._heaterPIDTuner.calc_final_pid()
                    self._heaterPIDTuner = None

        print("Closing")

    def stop(self):
        print("Controller: Stopping...")
        self.isRunning = False

    async def start(self):
        print("Controller: Starting...")

        self.isRunning = True
        await self.__run()
