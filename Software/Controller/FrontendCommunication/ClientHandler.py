import socket
import sys
import signal
import threading
import json
import copy
import asyncio
from pymitter import EventEmitter

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants

class ClientHandler:
  def __init__(self, eventEmitter: EventEmitter):
    self.eventEmitter = eventEmitter
    self.onRecievedMessageFromMicrocontroller = lambda *args: asyncio.create_task(self.__sendToClient(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, str(args[0])))
    self.onControllerStateMessage = lambda *args: asyncio.create_task(self.__sendToClient(constants.CONTROLLER_STATE_MESSAGE, args[0]))
    self._recieveBuffer = ''

  async def start(self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
    # signal.signal(signal.SIGINT, lambda *args: self.stop())
    # signal.signal(signal.SIGTERM, lambda *args: self.stop())

    print("FrontendComServer.Client: Connected")
    self.reader = reader
    self.writer = writer
    self.eventEmitter.on(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, self.onRecievedMessageFromMicrocontroller)
    self.eventEmitter.on(constants.CONTROLLER_STATE_MESSAGE, self.onControllerStateMessage)

    self.isRunning = True
    await self.__recieverThread()

  def stop(self):
    if (self.isRunning):
      print("FrontendComServer.Client: Stopping Frontend communication client...")
      self.isRunning = False
      self.unregisterEvent()
      self.writer.close()

  def unregisterEvent(self):
    self.eventEmitter.off(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, self.onRecievedMessageFromMicrocontroller)
    self.eventEmitter.off(constants.CONTROLLER_STATE_MESSAGE, self.onControllerStateMessage)

  async def __sendToClient(self, topic, data):
    try:
      #print("FrontendComServer.Client: sendToClient {msg}".format(msg = data))
      msg = {}
      msg['topic'] = topic
      msg['data'] = data
      jsonMessage = json.dumps(msg)
      self.writer.write(jsonMessage.encode('utf-8'))
      await self.writer.drain()
    except Exception as e:
        print("FrontendComServer.Client: Exception {e}".format(e=e))
        self.unregisterEvent()
        self.isRunning = False
        return      

  def __tryFindJSONMessage(self, str):
    numStartBracers = 0
    messageStart = -1
    messageEnd = -1
    for idx, val, in enumerate(str):
      if (val == '{'):
        numStartBracers += 1
        if (messageStart < 0):
          messageStart = idx
      if (val == '}'):
        numStartBracers -= 1
        if (messageEnd < 0):
          messageEnd = idx
          break
    return (messageStart, messageEnd)
      
  async def __recieverThread(self):
    while self.isRunning:
      try:
        data = await self.reader.read(1024*5)
        #addr = writer.get_extra_info('peername')

        if data == b'':
          print("FrontendComServer.Client disconnected")
          self.unregisterEvent()
          self.isRunning = False
          return

        if not data:
            break
        
        self._recieveBuffer += data.decode('utf-8')
        print("data: {d}".format(d = data))
        while True:
          messageStart, messageEnd = self.__tryFindJSONMessage(self._recieveBuffer)
          if (messageStart >= 0 and messageEnd >= 0):
            messageString = self._recieveBuffer[messageStart:messageEnd + 1]
            self._recieveBuffer = self._recieveBuffer[messageEnd + 1:]
            message = json.loads(messageString)
            print("incomming message: {msg}".format(msg = message))
            self.eventEmitter.emit(constants.FRONTEND_BASE_MESSAGE+message['topic'], message)
          else:
            break          
      except Exception as e:
        print("FrontendComServer.Client: Exception {e}".format(e=e))
        self.unregisterEvent()
        self.isRunning = False
        return