// Examples:
// Home: A2 I1
// Move To position: A5 G-9500
// Production: A1 I2
#ifndef OutgoingMessage_h
#define OutgoingMessage_h

#include "../Parameter.h"
#include "../Constants.h"

enum class OutgoingMessageType {
    Error = 'E',
    Telemetry = 'T',
    GetParams = 'G',
    Ack = 'A',
};

// struct OutgoingMessage {
//   OutgoingMessageType type;
//   int subType;
//   int id;
//   Parameter parameters[MAX_PARAMETERS];
//   int numParams;
// };

#endif