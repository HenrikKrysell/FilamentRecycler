// Examples:
// Home: A2 I1
// Move To position: A5 G-9500
// Production: A1 I2
#ifndef Protocol_h
#define Protocol_h

#define MAX_PARAMETERS 10
#define ID_PARAMETER_PREFIX 'I'

enum class MessageType {
    Error = 'E',
    Telemetry = 'T',
    PerformAction = 'A',
    SetParams = 'S',
    GetParams = 'G',
    Stop = 'P',
};

struct Parameter {
  char name;
  float floatValue;
  long longValue;
};

struct Message {
  MessageType type;
  int subType;
  int id;
  Parameter parameters[MAX_PARAMETERS];
  int numParams;
};

#endif