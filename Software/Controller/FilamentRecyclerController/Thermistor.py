import math
from .SampleQueue import SampleQueue

#################################################
# Thermistor class for a 100K thermistor with 4.7K pullup
##
# This code is taken from RepRap
# https://sourceforge.net/p/reprap/code/HEAD/tree/trunk/old_files/firmware/Arduino/utilities/createTemperatureLookup.py#l50
#################################################


class ThermistorCalculations:
    def __init__(self, r0: float, t0: float, beta: float, r1: float, r2: float):
        self.r0 = r0                        # stated resistance, e.g. 10K
        self.t0 = t0 + 273.15               # temperature at stated resistance, e.g. 25C
        self.beta = beta                    # stated beta, e.g. 3500
        self.vadc = 5.0                     # ADC reference
        self.vcc = 5.0                      # supply voltage to potential divider
        # constant part of calculation
        self.k = r0 * math.exp(-beta / self.t0)

        if r1 > 0:
            self.vs = r1 * self.vcc / (r1 + r2)  # effective bias voltage
            self.rs = r1 * r2 / (r1 + r2)       # effective bias impedance
        else:
            self.vs = self.vcc					 # effective bias voltage
            self.rs = r2                         # effective bias impedance

    def update(self, adc: int):
        v = adc * self.vadc / 1024          # convert the 10 bit ADC value to a voltage
        r = self.rs * v / (self.vs - v)     # resistance of thermistor
        self._temperatureC = (self.beta / math.log(r / self.k)
                              ) - 273.15        # temperature

    @property
    def temperatureC(self):
        return self._temperatureC


class Thermistor:
    def __init__(self, r0: float, t0: float, beta: float, r1: float, r2: float, numSamples: int):
        self._thermistorCalculations = ThermistorCalculations(
            r0, t0, beta, r1, r2)
        self._sampleQueue = SampleQueue(numSamples)

    def addSample(self, adc: int):
        self._thermistorCalculations.update(adc)
        self._sampleQueue.addSample(self._thermistorCalculations.temperatureC)

    @property
    def temperatureC(self):
        return self._sampleQueue.Average

    @property
    def NumSamples(self):
        return self._sampleQueue.SampleSize
