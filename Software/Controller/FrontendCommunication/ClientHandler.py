import socket
import sys
import signal
import threading
import json
import copy
import asyncio

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants

class ClientHandler:
  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

  async def start(self, reader, writer):
    # signal.signal(signal.SIGINT, lambda *args: self.stop())
    # signal.signal(signal.SIGTERM, lambda *args: self.stop())

    self.reader = reader
    self.writer = writer

    self.eventEmitter.on(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, lambda *args: asyncio.create_task(self.__sendToClient(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, args)))
    self.eventEmitter.on(constants.CONTROLLER_STATE_MESSAGE, lambda *args: asyncio.create_task(self.__sendToClient(constants.CONTROLLER_STATE_MESSAGE, args)))

    self.isRunning = True
    await self.__recieverThread()

  def stop(self):
    print("FrontendComServer.Client: Stopping Frontend communication client...")
    self.connection.close()
    self.isRunning = False
    #self.thread.stop()

  async def __sendToClient(self, topic, data):
    print("FrontendComServer.Client: sendToClient {msg}".format(msg = data))
    msg = {}
    msg['topic'] = topic
    msg['data'] = data
    jsonMessage = json.dumps(msg)
    self.writer.write(jsonMessage.encode('utf-8'))
    await self.writer.drain()

  async def __recieverThread(self):
    while self.isRunning:
      data = await self.reader.read(1024*5)
      #addr = writer.get_extra_info('peername')

      if not data:
          break

      message = json.loads(data.decode('utf-8'))
      self.eventEmitter.emit(constants.FRONTEND_BASE_MESSAGE+message['topic'], message)