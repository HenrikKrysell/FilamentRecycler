from .FromMicrocontrollerBaseMessage import FromMicrocontrollerBaseMessage

import sys, os, inspect
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0, parentdir)

class GetParamMessage(FromMicrocontrollerBaseMessage):
    def __init__(self, params):
        paramToPropertyList = {
            'C': '__ControllerState',
            'S': '__SpoolWinderState',
            'E': '__FilamentExtruderState',
            'P': '__PullerRPM',
            'X': '__ExtruderRPM',
            'T': '__Temperature',
            'G': '__FilamentGuidePos',
            'A': '__FilamentGuideStepsPerSpoolRevolution',
            'B': '__SpoolWinderNumStepsPerRevolution',
        }

        super().__init__(paramToPropertyList, params)

    @property
    def ControllerState(self):
        return self._getParam('C')

    @property
    def SpoolWinderState(self):
        return self._getParam('S')

    @property
    def FilamentExtruderState(self):
        return self._getParam('E')

    @property
    def PullerRPM(self):
        return self._getParam('P')

    @property
    def ExtruderRPM(self):
        return self._getParam('X')

    @property
    def Temperature(self):
        return self._getParam('T')

    @property
    def FilamentGuidePos(self):
        return self._getParam('G')

    @property
    def FilamentGuideStepsPerSpoolRevolution(self):
        return self._getParam('A')

    @property
    def SpoolWinderNumStepsPerRevolution(self):
        return self._getParam('B')
