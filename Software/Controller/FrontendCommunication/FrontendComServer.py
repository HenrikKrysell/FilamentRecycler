import socket
import sys
import signal
import json
from .ClientHandler import ClientHandler
import asyncio

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))

class FrontendComServer:

  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter
    self.clients = []

  async def start(self, port):
    print("FrontendComServer: Starting server")
    signal.signal(signal.SIGINT, lambda *args: self.stop())
    signal.signal(signal.SIGTERM, lambda *args: self.stop())

    HOST = '0.0.0.0'
    server = await asyncio.start_server(
        self._newClient, HOST, port, loop=asyncio.get_event_loop())

    addr = server.sockets[0].getsockname()
    print(f'FrontendComServer: Serving on {addr}')

    async with server:
        await server.serve_forever()

  def stop(self):
    print("FrontendComServer: Stopping server...")
    for client in self.clients:
      client.stop()

    self.isRunning = False

  async def _newClient(self, reader, writer):
    client = ClientHandler(self.eventEmitter)
    self.clients.append(client)
    await client.start(reader, writer)
