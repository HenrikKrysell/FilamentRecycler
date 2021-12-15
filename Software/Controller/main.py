#!/usr/bin/env python3

import json
import signal
from FilamentRecyclerController.controller import Controller
from FrontendCommunication.FrontendComServer import FrontendComServer
from pymitter import EventEmitter
from Microcontroller.SerialConnection import SerialConnection
from time import sleep
import constants
from Microcontroller.Messages.FromMicrocontroller.FromMicrocontrollerMessageFactory import FromMicrocontrollerMessageFactory
import asyncio
#import Microcontroller.Messages.ToMicrocontroller.RawStringMessage import RawStringMessage

# Gracefully close the app when docker closes
def handle_sigterm(*args):
    raise KeyboardInterrupt()
signal.signal(signal.SIGTERM, handle_sigterm)

def stop(controller: Controller, serialConnection: SerialConnection, frontend: FrontendComServer):
    controller.stop()
    serialConnection.stop()
    frontend.stop()

async def main():
    # DEBUG
    match = FromMicrocontrollerMessageFactory('T C1 S1 P456 X24 T200')
    print(match.toJson())

    eventEmitter = EventEmitter()

    controller = Controller(eventEmitter)
    controllerTask = asyncio.create_task(controller.start())

    serialConnection = SerialConnection(eventEmitter)
    serialConnectionTask = asyncio.create_task(serialConnection.start())

    frontendComServer = FrontendComServer(eventEmitter)
    frontendComServerTask =  asyncio.create_task(frontendComServer.start(5002))

    signal.signal(signal.SIGINT, lambda *args: stop(controller, serialConnection, frontendComServer))
    signal.signal(signal.SIGTERM, lambda *args: stop(controller, serialConnection, frontendComServer))

    # await asyncio.sleep(3)
    # setTelemetryMessage = RawStringMessage("T C P\n")
    # setTelmetryFrequencyMessage = RawStringMessage("S E500\n")
    # await setTelemetryMessage.sendToControllerAndWaitForResponse()
    # await setTelmetryFrequencyMessage.sendToControllerAndWaitForResponse()
    # #eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, "T C P\n")
    # #eventEmitter.emit(constants.SEND_MESSAGE_TO_MICROCONTROLLER, "S E500\n")

    await asyncio.wait([
        serialConnectionTask,
        controllerTask, 
        frontendComServerTask])

if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
    loop.close()
