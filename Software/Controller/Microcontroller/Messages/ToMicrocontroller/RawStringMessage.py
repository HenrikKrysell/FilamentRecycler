from .ToMicrocontrollerBaseMessage import ToMicrocontrollerBaseMessage

class RawStrngMessage(ToMicrocontrollerBaseMessage):
    def __init__(self, eventEmitter, msg):
        self._msg = msg
        super().__init__(eventEmitter, '', {})

    def _generateControllerMessage(self):
        return self._msg + ' I' + self._MessageId


