#!/usr/bin/env python3

import json
import signal
from FilamentRecyclerController.controller import Controller
from FrontendCommunication.FrontendComServer import FrontendComServer
from pymitter import EventEmitter
from Microcontroller.SerialConnection import SerialConnection
from time import sleep
from Microcontroller.MCSimulator import MCSimulator
from FrontendCommunication.Messages.ShutdownMessage import ShutdownMessage
import constants
import asyncio
import os
# import Microcontroller.Messages.ToMicrocontroller.RawStringMessage import RawStringMessage

# Gracefully close the app when docker closes


def handle_sigterm(*args):
    raise KeyboardInterrupt()


signal.signal(signal.SIGTERM, handle_sigterm)


def stop(controller: Controller, serialConnection: SerialConnection, frontend: FrontendComServer):
    controller.stop()
    serialConnection.stop()
    frontend.stop()


def shutdownServer(args):
    print("SHUTDOWN")
    os.system("sudo shutdown -h now")


async def main():
    eventEmitter = EventEmitter()

    eventEmitter.on(
        constants.FRONTEND_BASE_MESSAGE + ShutdownMessage.TOPIC, shutdownServer)

    controller = Controller(eventEmitter)
    controllerTask = asyncio.create_task(controller.start())

    # DEBUG
    #serialConnection = MCSimulator(eventEmitter)
    serialConnection = SerialConnection(eventEmitter)
    serialConnectionTask = asyncio.create_task(serialConnection.start())

    frontendComServer = FrontendComServer(eventEmitter)
    frontendComServerTask = asyncio.create_task(frontendComServer.start(5002))

    signal.signal(signal.SIGINT, lambda *args: stop(controller,
                  serialConnection, frontendComServer))
    signal.signal(signal.SIGTERM, lambda *args: stop(controller,
                  serialConnection, frontendComServer))

    await asyncio.wait([
        serialConnectionTask,
        controllerTask,
        frontendComServerTask])

if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
    loop.close()
