from .GetParamMessage import GetParamMessage

class TelemetricMessage(GetParamMessage):
    def __init__(self, params):
        super().__init__(params)
