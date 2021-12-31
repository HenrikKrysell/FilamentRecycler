class TelemetricDataMessage:
    def __init__(self, temperature: int, rpmCount: int, armPosition: int, filamentDiameter: int) -> None:
        self._temperature = temperature
        self._rpmCount = rpmCount
        self._armPosition = armPosition
        self._filamentDiameter = filamentDiameter

    @property
    def temperature(self):
        return self._temperature

    @property
    def rpmCount(self):
        return self._rpmCount

    @property
    def armPosition(self):
        return self._armPosition

    @property
    def filamentDiameter(self):
        return self._filamentDiameter

    def __repr__ (self):
        return str(self)
        
    def __str__(self):
        return 'Microcontroller::TelemetricData:: temp: {t}, rpmCount: {r}, armPosition: {a}, filamentDiameter: {f}'\
            .format(t = self.temperature, r = self.rpmCount, a = self.armPosition, f = self.filamentDiameter)
