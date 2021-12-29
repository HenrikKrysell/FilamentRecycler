import threading
import serial
import serial.tools.list_ports
from Microcontroller.Messages.ToMicrocontroller.ToMicrocontrollerBaseMessage import ToMicrocontrollerBaseMessage
import constants
import signal
import sys
import asyncio
import serial_asyncio
from bitstring import BitArray
from .Messages.FromMicrocontroller.MessageParser import parseMessage

class SerialConnection():

  class Connection(asyncio.Protocol):
    def __init__(self, eventEmitter):
      self.eventEmitter = eventEmitter
      self.buffer = BitArray()

      self.eventEmitter.on(constants.SEND_MESSAGE_TO_MICROCONTROLLER, lambda *args: self.__sendMessage(args[0]))

    def connection_made(self, transport):
        self.transport = transport
        print('SerialConnection: Connected to: ', transport)
        transport.serial.rts = False

    def consumeMessageBufferAndParseMessage(self):
        invalidMessageType = True
        parsedMessage = None
        insufficentBytes = False
        msgLength = 0
        while invalidMessageType and not insufficentBytes and self.buffer.length > 0:
          parsedMessage, msgLength, invalidMessageType, insufficentBytes = parseMessage(self.buffer)
          if invalidMessageType:
            del self.buffer[:8]

        if parsedMessage:
          del self.buffer[:msgLength]
        
        return parsedMessage

    def data_received(self, data: bytes):
      try:
        #print('New data: {d}'.format(d =data))
        self.buffer.append(data)

        while True:
          parsedMessage = self.consumeMessageBufferAndParseMessage()

          if parsedMessage:
              print("SerialConnection::{msg}".format(msg = parsedMessage))
              self.eventEmitter.emit(constants.RECIEVED_MESSAGE_FROM_MICROCONTROLLER, parsedMessage)
          else:
            break
      except Exception as error:
        #e = sys.exc_info()[0]
        print("SerialConnection: Error: %s" % error)

    def connection_lost(self, exc):
        print('SerialConnection: Connection closed')
        asyncio.get_event_loop().stop()

    def __sendMessage(self, msg: ToMicrocontrollerBaseMessage):
      self.transport.write(msg.generateControllerMessage())

  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

  def getAllComports(self):
    return [comport.device for comport in serial.tools.list_ports.comports()]

  def stop(self):
    print("SerialConnection: Stopping microcontroller communication...")
    self.isRunning = False

  async def start(self):
    print("SerialConnection: Starting Microcontroller communication...")
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
      baudrate=250000
    )
