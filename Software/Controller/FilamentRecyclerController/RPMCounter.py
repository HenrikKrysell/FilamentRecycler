class RPMCounter:
  def __init__(self, sampleSize: int, sampleTimeMs: int, numPerRev: int):
    self._queue = []
    self._sampleSize = sampleSize
    self._sampleTimeMs = sampleTimeMs
    self._currentRPM = 0
    self._numPerRev = numPerRev
  
  def addSample(self, count: int):
    if (len(self._queue) >= self._sampleSize):
      self._queue.pop(0)

    self._queue.append(count)
    self._currentRPM = (sum(self._queue) / self._numPerRev) * (60 / (len(self._queue) * (self._sampleTimeMs / 1000)))

  @property
  def currentRPM(self):
    return self._currentRPM
