from bitstring import BitArray

class ToMicrocontrollerBaseMessage:
    def generateControllerMessage(self) -> bytes:
        raise NotImplementedError()

# import asyncio
# import constants

# class ToMicrocontrollerBaseMessage:
#     _static_lastId = 0
#     _static_lastIdLock = asyncio.Lock()

#     def __init__(self, eventEmitter, messageType, paramToPropertyList):
#         paramToPropertyList['I'] = '_MessageId'
#         self._paramToPropertyList = paramToPropertyList
#         self._messageType = messageType
#         self._ackSemaphore = None        
#         self.eventEmitter = eventEmitter
#         self.eventEmitter.on(constants.RECIEVED_ACK_MESSAGE_FROM_MICROCONTROLLER, lambda *args: self.__ackMessageReceived(args))

#     def __ackMessageReceived(self, msg):
#         if self._ackSemaphore is not None and self._MessageId == msg.MessageId:
#             self._ackSemaphore.release()

#     async def sendToController(self):
#         async with ToMicrocontrollerBaseMessage._static_lastIdLock:
#             self._MessageId = ToMicrocontrollerBaseMessage._static_lastId
#             ToMicrocontrollerBaseMessage._static_lastId = ToMicrocontrollerBaseMessage._static_lastId + 1

#         self.eventEmitter.emit(MESSAGE_TO_MICROCONTROLLER, self._generateControllerMessage())

#     async def sendToControllerAndWaitForResponse(self):
#         async with ToMicrocontrollerBaseMessage._static_lastIdLock:
#             self._MessageId = ToMicrocontrollerBaseMessage._static_lastId
#             ToMicrocontrollerBaseMessage._static_lastId = ToMicrocontrollerBaseMessage._static_lastId + 1

#         self._ackSemaphore = asyncio.Semaphore(0)
#         self.eventEmitter.emit(MESSAGE_TO_MICROCONTROLLER, self._generateControllerMessage())
#         await self._ackSemaphore.acquire()

#     def _generateControllerMessage(self):
#         resultStr = self._messageType
#         for param in self._paramToPropertyList:
#             if hasattr(self._paramToPropertyList[param]):
#                 resultStr = resultStr + ' ' + param + str(getattr(self._paramToPropertyList[param]))
#         return resultStr
