import socket
import sys
import signal
import json
from .ClientHandler import ClientHandler
import asyncio
from pymitter import EventEmitter

import os,sys
sys.path.insert(1, os.path.join(sys.path[0], '..'))

class FrontendComServer:

  def __init__(self, eventEmitter: EventEmitter):
    self.eventEmitter = eventEmitter
    self.clients = []

  async def start(self, port: int):
    print("FrontendComServer: Starting server")

    HOST = '0.0.0.0'
    self.server = await asyncio.start_server(
        self._newClient, HOST, port, loop=asyncio.get_event_loop())

    addr = self.server.sockets[0].getsockname()
    print(f'FrontendComServer: Serving on {addr}')

    async with self.server:
        await self.server.serve_forever()

  def stop(self):
    print("FrontendComServer: Stopping server...")
    for client in self.clients:
      client.stop()

    self.server.close()

  async def _newClient(self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
    client = ClientHandler(self.eventEmitter)
    self.clients.append(client)
    await client.start(reader, writer)
