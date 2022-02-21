PID_SETTLE_DELTA = 1.
PID_SETTLE_SLOPE = .1
PID_PARAM_BASE = 255.
KELVIN_TO_CELSIUS = -273.15
MAX_HEAT_TIME = 5.0
AMBIENT_TEMP = 25.


class Heater:
    def __init__(self, smooth_time):
        #self.heater = heater
        self.heater_max_power = 1.  # heater.get_max_power()
        # config.getfloat('pid_Kp') / PID_PARAM_BASE
        self.Kp = 47.707711553863504 / PID_PARAM_BASE
        # config.getfloat('pid_Ki') / PID_PARAM_BASE
        self.Ki = 0.9025762451585112 / PID_PARAM_BASE
        # config.getfloat('pid_Kd') / PID_PARAM_BASE
        self.Kd = 630.4247851401528 / PID_PARAM_BASE
        self.min_deriv_time = smooth_time  # heater.get_smooth_time()
        self.temp_integ_max = 0.
        if self.Ki:
            self.temp_integ_max = self.heater_max_power / self.Ki
        self.prev_temp = AMBIENT_TEMP
        self.prev_temp_time = 0.
        self.prev_temp_deriv = 0.
        self.prev_temp_integ = 0.
        self._target_temp = 0.

    @property
    def target_temp(self):
        return self._target_temp

    @target_temp.setter
    def target_temp(self, value: float):
        self._target_temp = value

    def temperature_update(self, read_time, temp):
        time_diff = read_time - self.prev_temp_time
        # Calculate change of temperature
        temp_diff = temp - self.prev_temp
        if time_diff >= self.min_deriv_time:
            temp_deriv = temp_diff / time_diff
        else:
            temp_deriv = (self.prev_temp_deriv * (self.min_deriv_time-time_diff)
                          + temp_diff) / self.min_deriv_time
        # Calculate accumulated temperature "error"
        temp_err = self.target_temp - temp
        temp_integ = self.prev_temp_integ + temp_err * time_diff
        temp_integ = max(0., min(self.temp_integ_max, temp_integ))
        # Calculate output
        co = self.Kp*temp_err + self.Ki*temp_integ - self.Kd*temp_deriv
        # logging.debug("pid: %f@%.3f -> diff=%f deriv=%f err=%f integ=%f co=%d",
        #    temp, read_time, temp_diff, temp_deriv, temp_err, temp_integ, co)
        bounded_co = max(0., min(self.heater_max_power, co))
        #self.heater.set_pwm(read_time, bounded_co)
        # Store state for next measurement
        self.prev_temp = temp
        self.prev_temp_time = read_time
        self.prev_temp_deriv = temp_deriv
        if co == bounded_co:
            self.prev_temp_integ = temp_integ

        return bounded_co

    def check_busy(self, eventtime, smoothed_temp, target_temp):
        temp_diff = target_temp - smoothed_temp
        return (abs(temp_diff) > PID_SETTLE_DELTA
                or abs(self.prev_temp_deriv) > PID_SETTLE_SLOPE)
