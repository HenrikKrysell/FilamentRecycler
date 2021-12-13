#!/usr/bin/env python3

from .ErrorMessage import ErrorMessage
from .AckMessage import AckMessage
from .GetParamMessage import GetParamMessage
from .TelemetricMessage import TelemetricMessage
from .FromMicrocontrollerBaseMessage import FromMicrocontrollerBaseMessage
from .DebugMessage import DebugMessage
import re

regex = '([A-Z]\d*)'
regexComp = re.compile(regex)

def FromMicrocontrollerMessageFactory(rawMessage) -> FromMicrocontrollerBaseMessage:
    matches = regexComp.findall(rawMessage)
    if (len(matches) == 0):
        return DebugMessage(rawMessage)
    if (matches[0] == 'T'):
        return TelemetricMessage(matches[1:])
    if (matches[0] == 'G'):
        return GetParamMessage(matches[1:])
    if (matches[0] == 'A'):
        return AckMessage(matches[1:])
    if (matches[0] == 'E'):
        return ErrorMessage(matches[1:])

