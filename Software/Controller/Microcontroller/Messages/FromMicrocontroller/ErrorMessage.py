from enum import IntEnum
# import sys, os, inspect
# currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
# parentdir = os.path.dirname(currentdir)
# sys.path.insert(0, parentdir)

class ERROR_CODE(IntEnum):
    INVALID_INITIALIZE_MESSAGE = 1,

class ErrorMessage:    
    def __init__(self, errorCode: ERROR_CODE):
        self.errorCode = errorCode

    @property
    def ErrorCode(self) -> ERROR_CODE:
        return self.errorCode
    
    def __str__(self):
        if self.errorCode == ERROR_CODE.INVALID_INITIALIZE_MESSAGE:
            return 'Microcontroller::ERROR::INVALID_INITIALIZE_MESSAGE'

        return 'Microcontroller::ERROR::{code}'.format(code = self.errorCode)
