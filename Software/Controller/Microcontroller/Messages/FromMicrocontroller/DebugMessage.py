class DebugMessage:    
    def __init__(self, message: str):
        self._message = message

    @property
    def message(self) -> str:
        return self._message
    
    def __str__(self):
        return 'Microcontroller::DEBUG::{m}'.format(m = self.message)
