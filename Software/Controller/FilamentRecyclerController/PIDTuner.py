#############################################################################
# Most of this code has been taken from klipper
# https://github.com/Klipper3d/klipper/blob/master/klippy/extras/pid_calibrate.py
#############################################################################

# Autotune: raw=%f/%f Ku=%f Tu=%f  Kp=%f Ki=%f Kd=%f 8.166631515097436 1.0 0.3118151081951863 105.71452951431274 47.707711553863504 0.9025762451585112 630.4247851401528
# raw=%f/%f = 8.166631515097436 / 1.0
# Ku=%f = 0.3118151081951863
# Tu=%f = 105.71452951431274
# Kp=%f = 47.707711553863504
# Ki=%f = 0.9025762451585112
# Kd=%f = 630.4247851401528

from xmlrpc.client import boolean
import math

TUNE_PID_DELTA = 5.0
PID_PARAM_BASE = 255.


class PIDTuner:
    def __init__(self, targetTemperature):
        self.heater_max_power = 1.0
        self._calibrate_temp = targetTemperature
        self._currentTargetTemp = targetTemperature
        # Heating control
        self._heating = False
        self._peak = 0.
        self._peak_time = 0.
        # Peak recording
        self._peaks = []
        # Sample recording
        self._temp_samples = []

    @property
    def Heating(self) -> boolean:
        return self._heating

    def temperature_update(self, read_time, temp):
        self._temp_samples.append((read_time, temp))
        # Check if the temperature has crossed the target and
        # enable/disable the heater if so.
        if self._heating and temp >= self._currentTargetTemp:
            self._heating = False
            self.check_peaks()
            self._currentTargetTemp = self._calibrate_temp - TUNE_PID_DELTA
        elif not self._heating and temp <= self._currentTargetTemp:
            self._heating = True
            self.check_peaks()
            self._currentTargetTemp = self._calibrate_temp
        # Check if this temperature is a peak and record it if so
        if self._heating:
            #self.set_pwm(read_time, self.heater_max_power)
            if temp < self._peak:
                self._peak = temp
                self._peak_time = read_time
        else:
            #self.set_pwm(read_time, 0.)
            if temp > self._peak:
                self._peak = temp
                self._peak_time = read_time

    def check_busy(self):
        if self._heating or len(self._peaks) < 12:
            return True
        return False

    # Analysis
    def check_peaks(self):
        self._peaks.append((self._peak, self._peak_time))
        if self._heating:
            self._peak = 9999999.
        else:
            self._peak = -9999999.
        if len(self._peaks) < 4:
            return
        self.calc_pid(len(self._peaks)-1)

    def calc_pid(self, pos):
        temp_diff = self._peaks[pos][0] - self._peaks[pos-1][0]
        time_diff = self._peaks[pos][1] - self._peaks[pos-2][1]
        # Use Astrom-Hagglund method to estimate Ku and Tu
        amplitude = .5 * abs(temp_diff)
        Ku = 4. * self.heater_max_power / (math.pi * amplitude)
        Tu = time_diff
        # Use Ziegler-Nichols method to generate PID parameters
        Ti = 0.5 * Tu
        Td = 0.125 * Tu
        Kp = 0.6 * Ku * PID_PARAM_BASE
        Ki = Kp / Ti
        Kd = Kp * Td
        print("Autotune: raw=%f/%f Ku=%f Tu=%f  Kp=%f Ki=%f Kd=%f",
              temp_diff, self.heater_max_power, Ku, Tu, Kp, Ki, Kd)
        return Kp, Ki, Kd

    def calc_final_pid(self):
        cycle_times = [(self._peaks[pos][1] - self._peaks[pos-2][1], pos)
                       for pos in range(4, len(self._peaks))]
        midpoint_pos = sorted(cycle_times)[len(cycle_times)//2][1]
        return self.calc_pid(midpoint_pos)

    # # Offline analysis helper
    # def write_file(self, filename):
    #     pwm = ["pwm: %.3f %.3f" % (time, value)
    #            for time, value in self.pwm_samples]
    #     out = ["%.3f %.3f" % (time, temp) for time, temp in self.temp_samples]
    #     f = open(filename, "wb")
    #     f.write('\n'.join(pwm + out))
    #     f.close()
