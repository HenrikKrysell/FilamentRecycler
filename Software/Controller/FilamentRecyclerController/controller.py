import threading
from time import sleep
import serial
import serial.tools.list_ports
import time
import signal

class Controller:

  def getAllComports(self):
    return [comport.device for comport in serial.tools.list_ports.comports()]

  def __run(self):
    print([comport.device for comport in serial.tools.list_ports.comports()])

    lastTime = time.time()
    while self.isRunning:
      sleep(0.001)
      if (time.time() - lastTime > 10):
        print("controller...." + str(threading.get_ident()))
        lastTime = time.time()

    print("Closing")

  def stop(self):
    print("Stopping controller...")
    self.isRunning = False
    self.thread.stop()

  def start(self):
    print("Starting controller...")
    signal.signal(signal.SIGINT, lambda *args: self.stop())
    signal.signal(signal.SIGTERM, lambda *args: self.stop())

    self.isRunning = True
    self.thread = threading.Thread(target=self.__run)
    self.thread.start()