from xmlrpc.client import boolean
import bitstring
from .ToMicrocontrollerBaseMessage import ToMicrocontrollerBaseMessage


class SetValuesMessage(ToMicrocontrollerBaseMessage):
    def __init__(self) -> None:
        super().__init__()

        self._augerMotorSpeed = 0
        self._winderStepperRPM = 0
        self._pullerStepperRPM = 0
        self._filamentGuideSteps = 0
        self._turnOnHeater = False
        self._emergencyStop = False

    @property
    def emergencyStop(self) -> bool:
        return self._emergencyStop

    @emergencyStop.setter
    def emergencyStop(self, value: bool):
        self._emergencyStop = value

    @property
    def turnOnHeater(self) -> bool:
        return self._turnOnHeater

    @turnOnHeater.setter
    def turnOnHeater(self, value: bool):
        self._turnOnHeater = value

    @property
    def augerMotorSpeed(self) -> int:
        return self._augerMotorSpeed

    @augerMotorSpeed.setter
    def augerMotorSpeed(self, value: int):
        self._augerMotorSpeed = value

    @property
    def winderStepperRPM(self) -> int:
        return self._winderStepperRPM

    @winderStepperRPM.setter
    def winderStepperRPM(self, value: int):
        self._winderStepperRPM = value

    @property
    def pullerStepperRPM(self) -> int:
        return self._pullerStepperRPM

    @pullerStepperRPM.setter
    def pullerStepperRPM(self, value: int):
        self._pullerStepperRPM = value

    @property
    def filamentGuideSteps(self) -> int:
        return self._filamentGuideSteps

    @filamentGuideSteps.setter
    def filamentGuideSteps(self, value: int):
        self._filamentGuideSteps = value

    def generateControllerMessage(self) -> bytes:
        header = bitstring.pack(
            'bool, bool, bool, bool, uint:4',
            False,
            False,
            self.emergencyStop,
            self.turnOnHeater,
            9)

        bits = header + bitstring.pack(
            '>BBBB',
            self.augerMotorSpeed,
            self.winderStepperRPM,
            self.pullerStepperRPM,
            self.filamentGuideSteps)

        return bits.tobytes()
