#ifndef Outgoing_Utils_h
#define Outgoing_Utils_h

#include "MessageTypes.h"

void sendError(ERROR_CODE errorCode) {
  char buff[2] = {OUTGOING_MESSAGE_TYPE::ERROR, errorCode};
  Serial.write(buff, 2);
}

void sendMessage(OUTGOING_MESSAGE_TYPE msgType) {
  char buff[1] = {msgType};
  Serial.write(buff, 1);
}

void sendTelemetricData(int temperature, byte rpmCount, int armPosition, int filamentDiameter) {
  // TODO! by using 10 bits per int instead of 16 we will save 2 bytes and go from 77 us per send to 50us
  char buff[8] = {
    OUTGOING_MESSAGE_TYPE::TELEMETRIC_DATA,
    ((char*)&temperature)[0],
    ((char*)&temperature)[1],
    (char)rpmCount,
    ((char*)&armPosition)[0],
    ((char*)&armPosition)[1],
    ((char*)&filamentDiameter)[0],
    ((char*)&filamentDiameter)[1]};
  Serial.write(buff, 8);
}

#endif