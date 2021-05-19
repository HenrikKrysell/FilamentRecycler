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
  int intValue;
};

struct Message {
  MessageType type;
  int subType;
  int id;
  Parameter parameters[MAX_PARAMETERS];
  int numParams;
};

#endif