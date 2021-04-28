import socket
import sys
import signal
import threading
import json
from .ClientHandler import ClientHandler

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))
import constants

class FrontendComServer:

  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter
    self.clients = []

  def init(self, port):
    signal.signal(signal.SIGINT, lambda *args: self.stop())
    signal.signal(signal.SIGTERM, lambda *args: self.stop())

    self.port = port
    self.isRunning = True
    self.thread = threading.Thread(target=self.__recieverThread)
    self.thread.start()

  def stop(self):
    print("Stopping UpdComServer...")
    for client in self.clients:
      client.stop()

    self.isRunning = False
    self.thread.stop()

  def __recieverThread(self):
    HOST = '0.0.0.0'
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, self.port))
        s.listen()
        conn, addr = s.accept()
        print('Connected by', addr)
        client = ClientHandler(self.eventEmitter, conn)
        client.init()
        self.clients.append(client)
        # with conn:
        #     while True:
        #         data = conn.recv(1024)
        #         if not data:
        #             break
        #         print(data)
        #         message = json.loads(data.decode('utf-8'))
        #         print(message)
        #         self.eventEmitter.emit(constants.FRONTEND_BASE_MESSAGE+message['topic'], message)
        #         #conn.sendall(data)