from .FromMicrocontrollerBaseMessage import FromMicrocontrollerBaseMessage
from enum import Enum

class DebugMessage(FromMicrocontrollerBaseMessage):
    def __init__(self, message):
        paramToPropertyList = {
        }

        super().__init__(paramToPropertyList, "")
        self._message = message


    @property
    def Message(self) -> str:
        return self._message