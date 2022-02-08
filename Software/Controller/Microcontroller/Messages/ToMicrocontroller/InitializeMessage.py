import bitstring
from .ToMicrocontrollerBaseMessage import ToMicrocontrollerBaseMessage


class StepperConfig:
    def __init__(self) -> None:
        self._enablePin = 0
        self._stepPin = 0
        self._dirPin = 0
        self._dirValue = 0

    @property
    def enablePin(self):
        return self._enablePin

    @enablePin.setter
    def enablePin(self, value: int):
        self._enablePin = value

    @property
    def stepPin(self):
        return self._stepPin

    @stepPin.setter
    def stepPin(self, value: int):
        self._stepPin = value

    @property
    def dirPin(self):
        return self._dirPin

    @dirPin.setter
    def dirPin(self, value: int):
        self._dirPin = value

    # 0 = clockwise, 1 = Counter clockwise
    @property
    def dirValue(self):
        return self._dirValue

    @dirValue.setter
    def dirValue(self, value: int):
        self._dirValue = value

    def toBitstring(self) -> bitstring:
        return bitstring.pack(
            '<BBBB',
            self.enablePin,
            self.stepPin,
            self.dirPin,
            self.dirValue,
        )


class OlimexMotorDriverConfig:
    def __init__(self) -> None:
        self._inAPin = 0
        self._inBPin = 0
        self._pwmPin = 0
        self._dir = 0

    @property
    def inAPin(self):
        return self._inAPin

    @inAPin.setter
    def inAPin(self, value: int):
        self._inAPin = value

    @property
    def inBPin(self):
        return self._inBPin

    @inBPin.setter
    def inBPin(self, value: int):
        self._inBPin = value

    @property
    def pwmPin(self):
        return self._pwmPin

    @pwmPin.setter
    def pwmPin(self, value: int):
        self._pwmPin = value

    # 0 = clockwise, 1 = Counter clockwise
    @property
    def dir(self):
        return self._dir

    @dir.setter
    def dir(self, value: int):
        self._dir = value

    def toBitstring(self) -> bitstring:
        return bitstring.pack(
            '<BBBB',
            self.inAPin,
            self.inBPin,
            self.pwmPin,
            self.dir,
        )


class AnalogInputConfig:
    def __init__(self) -> None:
        self._pollIntervalMs = 0
        self._delayBetweenFirstPoll = 0
        self._delayBetweenSamples = 0
        self._pin = 0
        self._numSamples = 0

    @property
    def pollIntervalMs(self):
        return self._pollIntervalMs

    @pollIntervalMs.setter
    def pollIntervalMs(self, value: int):
        self._pollIntervalMs = value

    @property
    def delayBetweenFirstPoll(self):
        return self._delayBetweenFirstPoll

    @delayBetweenFirstPoll.setter
    def delayBetweenFirstPoll(self, value: int):
        self._delayBetweenFirstPoll = value

    @property
    def delayBetweenSamples(self):
        return self._delayBetweenSamples

    @delayBetweenSamples.setter
    def delayBetweenSamples(self, value: int):
        self._delayBetweenSamples = value

    @property
    def pin(self):
        return self._pin

    @pin.setter
    def pin(self, value: int):
        self._pin = value

    @property
    def numSamples(self):
        return self._numSamples

    @numSamples.setter
    def numSamples(self, value: int):
        self._numSamples = value

    def toBitstring(self) -> bitstring:
        return bitstring.pack(
            '<hhhBB',
            self.pollIntervalMs,
            self.delayBetweenFirstPoll,
            self.delayBetweenSamples,
            self.pin,
            self.numSamples,
        )


class CounterConfig:
    def __init__(self) -> None:
        self._pin = 0
        self._pollIntervalMs = 0
        self._delayBetweenFirstPoll = 0

    @property
    def pin(self):
        return self._pin

    @pin.setter
    def pin(self, value: int):
        self._pin = value

    @property
    def pollIntervalMs(self):
        return self._pollIntervalMs

    @pollIntervalMs.setter
    def pollIntervalMs(self, value: int):
        self._pollIntervalMs = value

    @property
    def delayBetweenFirstPoll(self):
        return self._delayBetweenFirstPoll

    @delayBetweenFirstPoll.setter
    def delayBetweenFirstPoll(self, value: int):
        self._delayBetweenFirstPoll = value

    def toBitstring(self) -> bitstring:
        return bitstring.pack(
            '<Bhh',
            self.pin,
            self.pollIntervalMs,
            self.delayBetweenFirstPoll,
        )


class InitializeMessage(ToMicrocontrollerBaseMessage):
    def __init__(self) -> None:
        super().__init__()

        self._heaterPin = 0
        self._filamentGuideEndstopPin = 0
        self._telemetricDataIntervalMs = 0
        self._motorDriver = OlimexMotorDriverConfig()
        self._motorRPMCounter = CounterConfig()
        self._pullerStepper = StepperConfig()
        self._winderStepper = StepperConfig()
        self._filamentGuideStepper = StepperConfig()
        self._thermistorConfig = AnalogInputConfig()
        self._winderArmConfig = AnalogInputConfig()
        self._filamentDiamterConfig = AnalogInputConfig()

    @property
    def motorDriver(self):
        return self._motorDriver

    @property
    def motorRPMCounter(self):
        return self._motorRPMCounter

    @property
    def pullerStepper(self):
        return self._pullerStepper

    @property
    def winderStepper(self):
        return self._winderStepper

    @property
    def filamentGuideStepper(self):
        return self._filamentGuideStepper

    @property
    def thermistorConfig(self):
        return self._thermistorConfig

    @property
    def winderArmConfig(self):
        return self._winderArmConfig

    @property
    def filamentDiamterConfig(self):
        return self._filamentDiamterConfig

    @property
    def heaterPin(self):
        return self._heaterPin

    @heaterPin.setter
    def heaterPin(self, value: int):
        self._heaterPin = value

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

    def generateControllerMessage(self) -> bytes:
        bits = bitstring.pack(
            '<BBBh',
            8,
            self.heaterPin,
            self.filamentGuideEndstopPin,
            self.telemetricDataIntervalMs) \
            + self.motorDriver.toBitstring() \
            + self.motorRPMCounter.toBitstring() \
            + self.pullerStepper.toBitstring() \
            + self.winderStepper.toBitstring() \
            + self.filamentGuideStepper.toBitstring() \
            + self.thermistorConfig.toBitstring() \
            + self.winderArmConfig.toBitstring() \
            + self.filamentDiamterConfig.toBitstring()

        return bits.tobytes()


# class InitializeMessage(ToMicrocontrollerBaseMessage):
#     def __init__(self) -> None:
#         super().__init__()

#         self._motorINAPin = 0
#         self._motorINBPin = 0
#         self._motorPWMPin = 0
#         self._motorDir = 0
#         self._motorRPMCounterPin = 0

#         self._pullerStepperEnablePin = 0
#         self._pullerStepperStepPin = 0
#         self._pullerStepperDirPin = 0
#         self._pullerStepperDirValue = 0

#         self._winderStepperEnablePin = 0
#         self._winderStepperStepPin = 0
#         self._winderStepperDirPin = 0
#         self._winderStepperDirValue = 0

#         self._filamentGuideStepperEnablePin = 0
#         self._filamentGuideStepperStepPin = 0
#         self._filamentGuideStepperDirPin = 0
#         self._filamentGuideStepperDirValue = 0

#         self._thermistorAnalogPin = 0
#         self._heaterPin = 0
#         self._winderArmAnalogPin = 0
#         self._filamentDiameterAnalogPin = 0
#         self._filamentGuideEndstopPin = 0

#         self._telemetricDataIntervalMs = 0
#         self._thermistorPollIntervalMs = 0
#         self._winderArmPollIntervalMs = 0
#         self._filamentDiameterPollIntervalMs = 0
#         self._motorRPMCounterPollIntervalMs = 0

#     @property
#     def motorINAPin(self):
#         return self._motorINAPin

#     @motorINAPin.setter
#     def motorINAPin(self, value: int):
#         self._motorINAPin = value

#     @property
#     def motorINBPin(self):
#         return self._motorINBPin

#     @motorINBPin.setter
#     def motorINBPin(self, value: int):
#         self._motorINBPin = value

#     @property
#     def motorPWMPin(self):
#         return self._motorPWMPin

#     @motorPWMPin.setter
#     def motorPWMPin(self, value: int):
#         self._motorPWMPin = value

#     @property
#     def motorDir(self):
#         return self._motorDir

#     @motorDir.setter
#     def motorDir(self, value: int):
#         self._motorDir = value

#     @property
#     def motorRPMCounterPin(self):
#         return self._motorRPMCounterPin

#     @motorRPMCounterPin.setter
#     def motorRPMCounterPin(self, value: int):
#         self._motorRPMCounterPin = value

#     @property
#     def pullerStepperEnablePin(self):
#         return self._pullerStepperEnablePin

#     @pullerStepperEnablePin.setter
#     def pullerStepperEnablePin(self, value: int):
#         self._pullerStepperEnablePin = value

#     @property
#     def pullerStepperStepPin(self):
#         return self._pullerStepperStepPin

#     @pullerStepperStepPin.setter
#     def pullerStepperStepPin(self, value: int):
#         self._pullerStepperStepPin = value

#     @property
#     def pullerStepperDirPin(self):
#         return self._pullerStepperDirPin

#     @pullerStepperDirPin.setter
#     def pullerStepperDirPin(self, value: int):
#         self._pullerStepperDirPin = value

#     @property
#     def pullerStepperDirValue(self):
#         return self._pullerStepperDirValue

#     @pullerStepperDirValue.setter
#     def pullerStepperDirValue(self, value: int):
#         self._pullerStepperDirValue = value

#     @property
#     def winderStepperEnablePin(self):
#         return self._winderStepperEnablePin

#     @winderStepperEnablePin.setter
#     def winderStepperEnablePin(self, value: int):
#         self._winderStepperEnablePin = value

#     @property
#     def winderStepperStepPin(self):
#         return self._winderStepperStepPin

#     @winderStepperStepPin.setter
#     def winderStepperStepPin(self, value: int):
#         self._winderStepperStepPin = value

#     @property
#     def winderStepperDirPin(self):
#         return self._winderStepperDirPin

#     @winderStepperDirPin.setter
#     def winderStepperDirPin(self, value: int):
#         self._winderStepperDirPin = value

#     @property
#     def winderStepperDirValue(self):
#         return self._winderStepperDirValue

#     @winderStepperDirValue.setter
#     def winderStepperDirValue(self, value: int):
#         self._winderStepperDirValue = value

#     @property
#     def filamentGuideStepperEnablePin(self):
#         return self._filamentGuideStepperEnablePin

#     @filamentGuideStepperEnablePin.setter
#     def filamentGuideStepperEnablePin(self, value: int):
#         self._filamentGuideStepperEnablePin = value

#     @property
#     def filamentGuideStepperStepPin(self):
#         return self._filamentGuideStepperStepPin

#     @filamentGuideStepperStepPin.setter
#     def filamentGuideStepperStepPin(self, value: int):
#         self._filamentGuideStepperStepPin = value

#     @property
#     def filamentGuideStepperDirPin(self):
#         return self._filamentGuideStepperDirPin

#     @filamentGuideStepperDirPin.setter
#     def filamentGuideStepperDirPin(self, value: int):
#         self._filamentGuideStepperDirPin = value

#     @property
#     def filamentGuideStepperDirValue(self):
#         return self._filamentGuideStepperDirValue

#     @filamentGuideStepperDirValue.setter
#     def filamentGuideStepperDirValue(self, value: int):
#         self._filamentGuideStepperDirValue = value

#     @property
#     def thermistorAnalogPin(self):
#         return self._thermistorAnalogPin

#     @thermistorAnalogPin.setter
#     def thermistorAnalogPin(self, value: int):
#         self._thermistorAnalogPin = value

#     @property
#     def heaterPin(self):
#         return self._heaterPin

#     @heaterPin.setter
#     def heaterPin(self, value: int):
#         self._heaterPin = value

#     @property
#     def winderArmAnalogPin(self):
#         return self._winderArmAnalogPin

#     @winderArmAnalogPin.setter
#     def winderArmAnalogPin(self, value: int):
#         self._winderArmAnalogPin = value

#     @property
#     def filamentDiameterAnalogPin(self):
#         return self._filamentDiameterAnalogPin

#     @filamentDiameterAnalogPin.setter
#     def filamentDiameterAnalogPin(self, value: int):
#         self._filamentDiameterAnalogPin = value

#     @property
#     def filamentGuideEndstopPin(self):
#         return self._filamentGuideEndstopPin

#     @filamentGuideEndstopPin.setter
#     def filamentGuideEndstopPin(self, value: int):
#         self._filamentGuideEndstopPin = value

#     @property
#     def telemetricDataIntervalMs(self):
#         return self._telemetricDataIntervalMs

#     @telemetricDataIntervalMs.setter
#     def telemetricDataIntervalMs(self, value: int):
#         self._telemetricDataIntervalMs = value

#     @property
#     def thermistorPollIntervalMs(self):
#         return self._thermistorPollIntervalMs

#     @thermistorPollIntervalMs.setter
#     def thermistorPollIntervalMs(self, value: int):
#         self._thermistorPollIntervalMs = value

#     @property
#     def winderArmPollIntervalMs(self):
#         return self._winderArmPollIntervalMs

#     @winderArmPollIntervalMs.setter
#     def winderArmPollIntervalMs(self, value: int):
#         self._winderArmPollIntervalMs = value

#     @property
#     def filamentDiameterPollIntervalMs(self):
#         return self._filamentDiameterPollIntervalMs

#     @filamentDiameterPollIntervalMs.setter
#     def filamentDiameterPollIntervalMs(self, value: int):
#         self._filamentDiameterPollIntervalMs = value

#     @property
#     def motorRPMCounterPollIntervalMs(self):
#         return self._motorRPMCounterPollIntervalMs

#     @motorRPMCounterPollIntervalMs.setter
#     def motorRPMCounterPollIntervalMs(self, value: int):
#         self._motorRPMCounterPollIntervalMs = value

#     def generateControllerMessage(self) -> bytes:
#         bits = bitstring.pack(
#             '<BBBBBBBBBBBBBBBBBBBBBBBhhhhh',
#             8,
#             self.motorINAPin,
#             self.motorINBPin,
#             self.motorPWMPin,
#             self.motorDir,
#             self.motorRPMCounterPin,

#             self.pullerStepperEnablePin,
#             self.pullerStepperStepPin,
#             self.pullerStepperDirPin,
#             self.pullerStepperDirValue,

#             self.winderStepperEnablePin,
#             self.winderStepperStepPin,
#             self.winderStepperDirPin,
#             self.winderStepperDirValue,

#             self.filamentGuideStepperEnablePin,
#             self.filamentGuideStepperStepPin,
#             self.filamentGuideStepperDirPin,
#             self.filamentGuideStepperDirValue,

#             self.thermistorAnalogPin,
#             self.heaterPin,
#             self.winderArmAnalogPin,
#             self.filamentDiameterAnalogPin,
#             self.filamentGuideEndstopPin,

#             self.telemetricDataIntervalMs,
#             self.thermistorPollIntervalMs,
#             self.winderArmPollIntervalMs,
#             self.filamentDiameterPollIntervalMs,
#             self.motorRPMCounterPollIntervalMs)

#         return bits.tobytes()
