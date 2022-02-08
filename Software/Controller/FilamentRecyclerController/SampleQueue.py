class SampleQueue:
  def __init__(self, sampleSize: int):
    self._sampleSize =  sampleSize
    self._queue = []
    self._average = 0

  def addSample(self, value):
    if (len(self._queue) >= self._sampleSize):
      self._queue.pop(0)

    self._queue.append(value)
    self._average = sum(self._queue) / len(self._queue)

  @property
  def Average(self):
    return self._average