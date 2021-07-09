// Examples:
// Home: A2 I1
// Move To position: A5 G-9500
// Production: A1 I2
#ifndef IncommingMessage_h
#define IncommingMessage_h

#include "../Parameter.h"
#include "../Constants.h"

enum class IncommingMessageType {
    PerformAction = 'A',
    SetParams = 'S',
    GetParams = 'G',
    Stop = 'P',
    SetTelemtryParams = 'T',
};

struct IncommingMessage {
  IncommingMessageType type;
  int subType;
  int id;
  Parameter parameters[MAX_PARAMETERS];
  int numParams;
};

#endif