class SetTargetTemperatureMessage:
    TOPIC = 'setTargetTemperature'

    def __init__(self, dict: dict):
        self._targetTemperature = dict['targetTemperature']

    @property
    def targetTemperature(self):
        return self._targetTemperature
