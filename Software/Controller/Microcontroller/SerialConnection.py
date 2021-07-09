#import threading
import serial
import serial.tools.list_ports
import constants
import signal
import sys
import asyncio
import serial_asyncio
# from .Messages.FromMicrocontroller.TelemetricMessage import TelemetricMessage
# from .Messages.FromMicrocontroller.FromMicrocontrollerMessageFactory import FromMicrocontrollerMessageFactory

class SerialConnection():

  class Connection(asyncio.Protocol):
    def __init__(self, eventEmitter):
      self.eventEmitter = eventEmitter
      self.buffer = ""

      self.eventEmitter.on(constants.SEND_MESSAGE_TO_MICROCONTROLLER, lambda *args: self.__sendMessage(args))

    def connection_made(self, transport):
        self.transport = transport
        print('SerialConnection: Connected to: ', transport)
        transport.serial.rts = False

    def data_received(self, data):
      try:
        combinedData = self.buffer + data.decode('utf-8')
        lines = combinedData.split('\n')
        # save the last line as the buffer
        self.buffer = lines[-1]
        completeLines = lines[:-1]
        if (len(completeLines) >= 1):
          for line in completeLines:
            print("SerialConnection: Firmware: {msg}".format(msg = line))
            self.eventEmitter.emit(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, line)
      except:
        e = sys.exc_info()[0]
        print("SerialConnection: <p>Error: %s</p>" % e)

    def connection_lost(self, exc):
        print('SerialConnection: Connection closed')
        asyncio.get_event_loop().stop()

    def __sendMessage(self, data):
      self.transport.write(data[0].encode('utf-8'))

  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

  def getAllComports(self):
    return [comport.device for comport in serial.tools.list_ports.comports()]

  def stop(self):
    print("SerialConnection: Stopping microcontroller communication...")
    self.isRunning = False

  async def start(self):
    print("SerialConnection: Starting Microcontroller communication...")
    signal.signal(signal.SIGINT, lambda *args: self.stop())
    signal.signal(signal.SIGTERM, lambda *args: self.stop())
    comports = self.getAllComports()
    print(f"SerialConnection: Available COM ports: {comports!r}")
    
    if (len(comports) <= 0):
      print("SerialConnection: No available comports, quitting...")
      return

    comport = comports[0]
    print(f"SerialConnection: Selecting COM port: {comport!r}")

    await serial_asyncio.create_serial_connection(
      asyncio.get_event_loop(), 
      lambda *args: SerialConnection.Connection(self.eventEmitter),
      comport, 
      baudrate=115200
    )
