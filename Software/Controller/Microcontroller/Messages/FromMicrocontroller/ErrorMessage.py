from .FromMicrocontrollerBaseMessage import FromMicrocontrollerBaseMessage
from enum import Enum
# import sys, os, inspect
# currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
# parentdir = os.path.dirname(currentdir)
# sys.path.insert(0, parentdir)

class ErrorMessage(FromMicrocontrollerBaseMessage):
    def __init__(self, params):
        paramToPropertyList = {
            'C': '__ErrorCode',
            'N': '__IntParam',
        }

        super().__init__(paramToPropertyList, params)


    @property
    def IntParam(self) -> int:
        return self._getParam(self.paramToPropertyList['N'])

    @property
    def ErrorCode(self):
        return self._getParam(self.paramToPropertyList['C'])
