import bitstring
from .ToMicrocontrollerBaseMessage import ToMicrocontrollerBaseMessage

class InitializeMessage(ToMicrocontrollerBaseMessage):
    def __init__(self) -> None:
        super().__init__()

        self._motorINAPin = 0
        self._motorINBPin = 0
        self._motorPWMPin = 0
        self._motorDir = 0
        self._motorRPMCounterPin = 0

        self._pullerStepperEnablePin = 0
        self._pullerStepperStepPin = 0
        self._pullerStepperDirPin = 0
        self._pullerStepperDirValue = 0

        self._winderStepperEnablePin = 0
        self._winderStepperStepPin = 0
        self._winderStepperDirPin = 0
        self._winderStepperDirValue = 0

        self._filamentGuideStepperEnablePin = 0
        self._filamentGuideStepperStepPin = 0
        self._filamentGuideStepperDirPin = 0
        self._filamentGuideStepperDirValue = 0

        self._thermistorAnalogPin = 0
        self._heaterPin = 0
        self._winderArmAnalogPin = 0
        self._filamentDiameterAnalogPin = 0
        self._filamentGuideEndstopPin = 0

        self._telemetricDataIntervalMs = 0
        self._thermistorPollIntervalMs = 0
        self._winderArmPollIntervalMs = 0
        self._filamentDiameterPollIntervalMs = 0
        self._motorRPMCounterPollIntervalMs = 0


    @property
    def motorINAPin(self):
        return self._motorINAPin

    @motorINAPin.setter
    def motorINAPin(self, value: int):
        self._motorINAPin = value

    @property
    def motorINBPin(self):
        return self._motorINBPin

    @motorINBPin.setter
    def motorINBPin(self, value: int):
        self._motorINBPin = value

    @property
    def motorPWMPin(self):
        return self._motorPWMPin

    @motorPWMPin.setter
    def motorPWMPin(self, value: int):
        self._motorPWMPin = value

    @property
    def motorDir(self):
        return self._motorDir

    @motorDir.setter
    def motorDir(self, value: int):
        self._motorDir = value

    @property
    def motorRPMCounterPin(self):
        return self._motorRPMCounterPin

    @motorRPMCounterPin.setter
    def motorRPMCounterPin(self, value: int):
        self._motorRPMCounterPin = value

    @property
    def pullerStepperEnablePin(self):
        return self._pullerStepperEnablePin

    @pullerStepperEnablePin.setter
    def pullerStepperEnablePin(self, value: int):
        self._pullerStepperEnablePin = value

    @property
    def pullerStepperStepPin(self):
        return self._pullerStepperStepPin

    @pullerStepperStepPin.setter
    def pullerStepperStepPin(self, value: int):
        self._pullerStepperStepPin = value

    @property
    def pullerStepperDirPin(self):
        return self._pullerStepperDirPin

    @pullerStepperDirPin.setter
    def pullerStepperDirPin(self, value: int):
        self._pullerStepperDirPin = value

    @property
    def pullerStepperDirValue(self):
        return self._pullerStepperDirValue

    @pullerStepperDirValue.setter
    def pullerStepperDirValue(self, value: int):
        self._pullerStepperDirValue = value

    @property
    def winderStepperEnablePin(self):
        return self._winderStepperEnablePin

    @winderStepperEnablePin.setter
    def winderStepperEnablePin(self, value: int):
        self._winderStepperEnablePin = value

    @property
    def winderStepperStepPin(self):
        return self._winderStepperStepPin

    @winderStepperStepPin.setter
    def winderStepperStepPin(self, value: int):
        self._winderStepperStepPin = value

    @property
    def winderStepperDirPin(self):
        return self._winderStepperDirPin

    @winderStepperDirPin.setter
    def winderStepperDirPin(self, value: int):
        self._winderStepperDirPin = value

    @property
    def winderStepperDirValue(self):
        return self._winderStepperDirValue

    @winderStepperDirValue.setter
    def winderStepperDirValue(self, value: int):
        self._winderStepperDirValue = value

    @property
    def filamentGuideStepperEnablePin(self):
        return self._filamentGuideStepperEnablePin

    @filamentGuideStepperEnablePin.setter
    def filamentGuideStepperEnablePin(self, value: int):
        self._filamentGuideStepperEnablePin = value

    @property
    def filamentGuideStepperStepPin(self):
        return self._filamentGuideStepperStepPin

    @filamentGuideStepperStepPin.setter
    def filamentGuideStepperStepPin(self, value: int):
        self._filamentGuideStepperStepPin = value

    @property
    def filamentGuideStepperDirPin(self):
        return self._filamentGuideStepperDirPin

    @filamentGuideStepperDirPin.setter
    def filamentGuideStepperDirPin(self, value: int):
        self._filamentGuideStepperDirPin = value

    @property
    def filamentGuideStepperDirValue(self):
        return self._filamentGuideStepperDirValue

    @filamentGuideStepperDirValue.setter
    def filamentGuideStepperDirValue(self, value: int):
        self._filamentGuideStepperDirValue = value

    @property
    def thermistorAnalogPin(self):
        return self._thermistorAnalogPin

    @thermistorAnalogPin.setter
    def thermistorAnalogPin(self, value: int):
        self._thermistorAnalogPin = value

    @property
    def heaterPin(self):
        return self._heaterPin

    @heaterPin.setter
    def heaterPin(self, value: int):
        self._heaterPin = value

    @property
    def winderArmAnalogPin(self):
        return self._winderArmAnalogPin

    @winderArmAnalogPin.setter
    def winderArmAnalogPin(self, value: int):
        self._winderArmAnalogPin = value

    @property
    def filamentDiameterAnalogPin(self):
        return self._filamentDiameterAnalogPin

    @filamentDiameterAnalogPin.setter
    def filamentDiameterAnalogPin(self, value: int):
        self._filamentDiameterAnalogPin = value

    @property
    def filamentGuideEndstopPin(self):
        return self._filamentGuideEndstopPin

    @filamentGuideEndstopPin.setter
    def filamentGuideEndstopPin(self, value: int):
        self._filamentGuideEndstopPin = value

    @property
    def telemetricDataIntervalMs(self):
        return self._telemetricDataIntervalMs

    @telemetricDataIntervalMs.setter
    def telemetricDataIntervalMs(self, value: int):
        self._telemetricDataIntervalMs = value

    @property
    def thermistorPollIntervalMs(self):
        return self._thermistorPollIntervalMs

    @thermistorPollIntervalMs.setter
    def thermistorPollIntervalMs(self, value: int):
        self._thermistorPollIntervalMs = value

    @property
    def winderArmPollIntervalMs(self):
        return self._winderArmPollIntervalMs

    @winderArmPollIntervalMs.setter
    def winderArmPollIntervalMs(self, value: int):
        self._winderArmPollIntervalMs = value

    @property
    def filamentDiameterPollIntervalMs(self):
        return self._filamentDiameterPollIntervalMs

    @filamentDiameterPollIntervalMs.setter
    def filamentDiameterPollIntervalMs(self, value: int):
        self._filamentDiameterPollIntervalMs = value

    @property
    def motorRPMCounterPollIntervalMs(self):
        return self._motorRPMCounterPollIntervalMs

    @motorRPMCounterPollIntervalMs.setter
    def motorRPMCounterPollIntervalMs(self, value: int):
        self._motorRPMCounterPollIntervalMs = value

    def generateControllerMessage(self) -> bytes:
        bits = bitstring.pack(\
            '<BBBBBBBBBBBBBBBBBBBBBBBhhhhh',\
            8,\
            self.motorINAPin,\
            self.motorINBPin,\
            self.motorPWMPin,\
            self.motorDir,\
            self.motorRPMCounterPin,\

            self.pullerStepperEnablePin, \
            self.pullerStepperStepPin, \
            self.pullerStepperDirPin, \
            self.pullerStepperDirValue, \
                
            self.winderStepperEnablePin, \
            self.winderStepperStepPin, \
            self.winderStepperDirPin, \
            self.winderStepperDirValue, \

            self.filamentGuideStepperEnablePin, \
            self.filamentGuideStepperStepPin, \
            self.filamentGuideStepperDirPin, \
            self.filamentGuideStepperDirValue, \

            self.thermistorAnalogPin, \
            self.heaterPin, \
            self.winderArmAnalogPin,\
            self.filamentDiameterAnalogPin,\
            self.filamentGuideEndstopPin,\

            self.telemetricDataIntervalMs, \
            self.thermistorPollIntervalMs,\
            self.winderArmPollIntervalMs,\
            self.filamentDiameterPollIntervalMs,\
            self.motorRPMCounterPollIntervalMs)

        return bits.tobytes()


