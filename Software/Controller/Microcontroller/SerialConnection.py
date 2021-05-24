import threading
import serial
import serial.tools.list_ports
import constants
import signal
import sys

class SerialConnection:
  def __init__(self, eventEmitter):
    self.eventEmitter = eventEmitter

    self.eventEmitter.on(constants.MESSAGE_TO_MICROCONTROLLER, lambda *args: self.__sendMessage(args))

  def getAllComports(self):
    return [comport.device for comport in serial.tools.list_ports.comports()]

  def __run(self):
    while self.isRunning:
      try:
        if not self.ser.is_open:
          print("Trying to open serial")
          self.ser.open()
          continue

        line = self.ser.readline().decode('utf-8')
        if line:
          print("Firmware: {msg}".format(msg = line))
          self.eventEmitter.emit(constants.MESSAGE_FROM_MICROCONTROLLER, line)
      except:
        e = sys.exc_info()[0]
        print("ERROR connecting to microcontroller: {e}".format(e = e))
        print("<p>Error: %s</p>" % e)

    self.ser.close()
    print("Closing")

  def __sendMessage(self, data):
    if self.ser.is_open:
      self.ser.write(data[0].encode('utf-8'))

  def stop(self):
    print("Stopping controller...")
    self.isRunning = False
    self.thread.stop()

  def start(self):
    print("Starting Microcontroller communication...")
    signal.signal(signal.SIGINT, lambda *args: self.stop())
    signal.signal(signal.SIGTERM, lambda *args: self.stop())

    self.ser = serial.Serial()
    self.ser.baudrate = 115200
    self.ser.port = '/dev/ttyUSB0'
    self.ser.timeout = 10

    self.isRunning = True
    self.thread = threading.Thread(target=self.__run)
    self.thread.start()

