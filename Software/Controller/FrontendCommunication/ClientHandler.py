import socket
import sys
import signal
import threading
import json
import copy

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants

class ClientHandler:
  def __init__(self, eventEmitter, connection):
    self.eventEmitter = eventEmitter
    self.connection = connection

  def init(self):
    # signal.signal(signal.SIGINT, lambda *args: self.stop())
    # signal.signal(signal.SIGTERM, lambda *args: self.stop())

    self.eventEmitter.on(constants.MESSAGE_FROM_MICROCONTROLLER, lambda *args: self.__sendToClient(constants.MESSAGE_FROM_MICROCONTROLLER, args))
    self.eventEmitter.on(constants.CONTROLLER_STATE_MESSAGE, lambda *args: self.__sendToClient(constants.CONTROLLER_STATE_MESSAGE, args))

    self.isRunning = True
    self.thread = threading.Thread(target=self.__recieverThread)
    self.thread.start()    

  def stop(self):
    print("Stopping Frontend communication client...")
    self.connection.close()
    self.isRunning = False
    self.thread.stop()

  def __sendToClient(self, topic, data):
    print("sendToClient {msg}".format(msg = data))
    msg = {}
    msg['topic'] = topic
    msg['data'] = data
    jsonMessage = json.dumps(msg)
    self.connection.sendall(jsonMessage.encode('utf-8'))

  def __recieverThread(self):
    with self.connection:
      while self.isRunning:
        data = self.connection.recv(1024*5)
        if not data:
            break

        message = json.loads(data.decode('utf-8'))
        self.eventEmitter.emit(constants.FRONTEND_BASE_MESSAGE+message['topic'], message)