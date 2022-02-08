#ifndef Outgoing_Utils_h
#define Outgoing_Utils_h

#include "MessageTypes.h"

struct MessageHeader {
    byte messageType: 4;
};

void sendError(ERROR_CODE errorCode) {
  MessageHeader header;
  header.messageType = OUTGOING_MESSAGE_TYPE::ERROR;
  char buff[2] = {((char*)&header)[0], errorCode};
  Serial.write(buff, 2);
}

void sendMessage(OUTGOING_MESSAGE_TYPE msgType) {
  MessageHeader header;
  header.messageType = msgType;
  Serial.write((char*)&header, sizeof(MessageHeader));
}

#endif