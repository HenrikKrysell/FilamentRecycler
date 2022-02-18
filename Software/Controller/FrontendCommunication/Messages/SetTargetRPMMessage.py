class SetTargetRPMMessage:
    TOPIC = 'setRPM'

    def __init__(self, dict: dict):
        self._targetRPM = dict['targetRPM']

    @property
    def targetRPM(self):
        return self._targetRPM
