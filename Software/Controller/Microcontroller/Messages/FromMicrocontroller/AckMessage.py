from .FromMicrocontrollerBaseMessage import FromMicrocontrollerBaseMessage
from enum import Enum
# import sys, os, inspect
# currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
# parentdir = os.path.dirname(currentdir)
# sys.path.insert(0, parentdir)

class AckResultType(Enum):
    SUCCESS = 1,
    ERROR = 2,
    STOPPED = 3,

class AckMessage(FromMicrocontrollerBaseMessage):
    def __init__(self, params):
        paramToPropertyList = {
            'S': '__Success',
            'E': '__Error',
            'T': '__Stopped',
        }

        super().__init__(paramToPropertyList, params)

        if hasattr(paramToPropertyList['S']):
            self._resultType = AckResultType.SUCCESS
        elif hasattr(paramToPropertyList['E']):
            self._resultType = AckResultType.ERROR
            self._error = self._getParam(paramToPropertyList['E'])
        elif hasattr(paramToPropertyList['T']):
            self._resultType = AckResultType.STOPPED


    @property
    def ResultType(self) -> AckResultType:
        return self._resultType

    @property
    def Error(self):
        return self._error