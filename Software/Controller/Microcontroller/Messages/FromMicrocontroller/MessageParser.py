from collections import namedtuple
from .DebugMessage import DebugMessage
from .TelemetricDataMessage import TelemetricDataMessage
from .WelcomeMessage import WelcomeMessage
from .ErrorMessage import ErrorMessage
from enum import IntEnum
from bitstring import BitArray
from typing import Match
import os
import sys
import inspect

from Microcontroller.Messages.FromMicrocontroller.InitializationCompleteMessage import InitializationCompleteMessage
currentdir = os.path.dirname(os.path.abspath(
    inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0, parentdir)


class INCOMMING_MESSAGE_TYPES(IntEnum):
    WELCOME = 0
    ERROR = 1
    INITIALIZE_DONE = 2
    TELEMETRIC_DATA = 3
    DEBUG_MESSAGE = 4


ParseMessageResult = namedtuple(
    'ParseMessageResult', 'msg msgLength invalidMessageType insufficentBytes')


def parseMessage(buffer: BitArray) -> ParseMessageResult:
    messageType = buffer[4:8].int
    if messageType == INCOMMING_MESSAGE_TYPES.WELCOME:
        return ParseMessageResult(WelcomeMessage(), 8, False, False)
    elif messageType == INCOMMING_MESSAGE_TYPES.ERROR:
        if (buffer.length < 16):
            return ParseMessageResult(None, 0, False, True)
        errorCode = buffer[8:16].int
        return ParseMessageResult(ErrorMessage(errorCode), 16, False, False)
    elif messageType == INCOMMING_MESSAGE_TYPES.INITIALIZE_DONE:
        return ParseMessageResult(InitializationCompleteMessage(), 8, False, False)
    elif messageType == INCOMMING_MESSAGE_TYPES.TELEMETRIC_DATA:
        numBits = 8*8  # 8*8
        if (buffer.length < numBits):
            return ParseMessageResult(None, 0, False, True)

        filamentGuideEndStopTriggered = buffer[3]
        # true if the filament guide stepper still has steps to move to reach set position
        filamentGuideStepperMoving = buffer[2]
        pos = 8
        temperature = buffer[pos:pos+16].intle
        pos += 16
        rpmCount = buffer[pos:pos + 8].uint
        pos += 8
        armPosition = buffer[pos:pos + 16].intle
        pos += 16
        filamentDiameter = buffer[pos: pos + 16].intle
        return ParseMessageResult(TelemetricDataMessage(temperature, rpmCount, armPosition, filamentDiameter, filamentGuideEndStopTriggered, filamentGuideStepperMoving), numBits, False, False)
        # return ParseMessageResult(TelemetricDataMessage(temperature, rpmCount, armPosition, 1), numBits, False, False)
    elif messageType == INCOMMING_MESSAGE_TYPES.DEBUG_MESSAGE:
        pos = buffer.find(hex(ord("'")), start=8, bytealigned=True)
        if pos:
            return ParseMessageResult(DebugMessage(buffer[8:pos[0]].bytes.decode('utf-8')), pos[0] + 8, False, False)

        return ParseMessageResult(None, 0, False, True)

    return ParseMessageResult(None, 0, True, False)
