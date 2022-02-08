from .SampleQueue import SampleQueue

class RPMCounter:
  def __init__(self, sampleSize: int, sampleTimeMs: int, numPerRev: int):
    self._sampleQueue = SampleQueue(sampleSize)
    self._sampleTimeMs = sampleTimeMs
    self._currentRPM = 0
    self._numPerRev = numPerRev
  
  def addSample(self, count: int):

    self._sampleQueue.addSample(count)
    #self._currentRPM = (sum(self._queue) / self._numPerRev) * (60 / (len(self._queue) * (self._sampleTimeMs / 1000)))
    self._currentRPM = self._sampleQueue.Average *  (60 / ((self._sampleTimeMs / 1000) * self._numPerRev))

  @property
  def currentRPM(self):
    return self._currentRPM
