from Microcontroller.Messages.ToMicrocontroller.ToMicrocontrollerBaseMessage import ToMicrocontrollerBaseMessage
from Microcontroller.Messages.FromMicrocontroller.WelcomeMessage import WelcomeMessage
from Microcontroller.Messages.FromMicrocontroller.InitializationCompleteMessage import InitializationCompleteMessage
from Microcontroller.Messages.FromMicrocontroller.TelemetricDataMessage import TelemetricDataMessage
from Microcontroller.Messages.ToMicrocontroller.SetValuesMessage import SetValuesMessage
import constants
import asyncio
import serial_asyncio
from bitstring import BitArray
from .Messages.FromMicrocontroller.MessageParser import parseMessage
import traceback
from pymitter import EventEmitter


class MCSimulator():
    def __init__(self, eventEmitter: EventEmitter):
        self.eventEmitter = eventEmitter
        self.heaterOn = False
        self.augerSpeed = 0
        self.isRunning = True

        self.eventEmitter.on(constants.SEND_MESSAGE_TO_MICROCONTROLLER,
                             lambda *args: self.__onSendMessageToMC(args[0]))

    def __onSendMessageToMC(self, msg):
        if (isinstance(msg, SetValuesMessage)):
            print("new message, heater: {h}".format(h=msg.turnOnHeater))
            self.heaterOn = msg.turnOnHeater
            self.augerSpeed = msg.augerMotorSpeed

    def stop(self):
        print("MCSimulator: Stopping...")
        self.isRunning = False

    async def start(self):
        print("MCSimulator: Starting...")
        await asyncio.sleep(1)

        welcomeMessage = WelcomeMessage()
        self.eventEmitter.emit(
            constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, welcomeMessage)
        await asyncio.sleep(1)
        initializationCompleteMessage = InitializationCompleteMessage()
        self.eventEmitter.emit(
            constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, initializationCompleteMessage)

        currentTemperature = 300  # 145C
        currentAugerSpeed = 0
        while self.isRunning:
            await asyncio.sleep(0.5)
            currentTemperature += -2 if self.heaterOn else 2
            currentAugerSpeed = 0
            telemetricDataMessage = TelemetricDataMessage(temperature=currentTemperature, rpmCount=0, armPosition=0,
                                                          filamentDiameter=0, filamentGuideEndStopTriggered=False, filamentGuideStepperMoving=False)

            self.eventEmitter.emit(
                constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, telemetricDataMessage)

        print("Closing")
