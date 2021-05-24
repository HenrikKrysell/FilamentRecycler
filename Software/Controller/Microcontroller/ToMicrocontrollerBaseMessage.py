import asyncio
import constants

class ToMicrocontrollerBaseMessage:
    def __init__(self, eventEmitter):
        self.eventEmitter = eventEmitter

    def sendToController(self, ):
        self.eventEmitter.emit(MESSAGE_TO_MICROCONTROLLER, self.__generateControllerMessage())

    def sendToControllerAndWaitForResponse(self, ):
        self.eventEmitter.emit(MESSAGE_TO_MICROCONTROLLER, self.__generateControllerMessage())

    def __generateControllerMessage(self):
        pass