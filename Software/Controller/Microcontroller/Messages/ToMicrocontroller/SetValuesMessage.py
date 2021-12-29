import bitstring
from .ToMicrocontrollerBaseMessage import ToMicrocontrollerBaseMessage

class SetValuesMessage(ToMicrocontrollerBaseMessage):
    def __init__(self) -> None:
        super().__init__()

        self._augerMotorSpeed = 0
        self._winderStepperRPM = 0
        self._pullerStepperRPM = 0
        self._filamentGuideSteps = 0

    @property
    def augerMotorSpeed(self):
        return self._augerMotorSpeed

    @augerMotorSpeed.setter
    def augerMotorSpeed(self, value: int):
        self._augerMotorSpeed = value

    @property
    def winderStepperRPM(self):
        return self._winderStepperRPM

    @winderStepperRPM.setter
    def winderStepperRPM(self, value: int):
        self._winderStepperRPM = value

    @property
    def pullerStepperRPM(self):
        return self._pullerStepperRPM

    @pullerStepperRPM.setter
    def pullerStepperRPM(self, value: int):
        self._pullerStepperRPM = value

    @property
    def filamentGuideSteps(self):
        return self._filamentGuideSteps

    @filamentGuideSteps.setter
    def filamentGuideSteps(self, value: int):
        self._filamentGuideSteps = value


    def generateControllerMessage(self) -> bytes:
        bits = bitstring.pack(\
            '>BBBBB',\
            9,\
            self.augerMotorSpeed,\
            self.winderStepperRPM,\
            self.pullerStepperRPM,\
            self.filamentGuideSteps)

        return bits.tobytes()


