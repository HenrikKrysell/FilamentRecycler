#ifndef Command_h
#define Command_h

#define MAX_PARAMETERS 10

enum CommandType {
  Nothing = 0,
  ChangeState = 1,
  SetSpeed = 2,
  SetTemperature = 4,
};

struct Parameter {
  char name;
  float floatValue;
  int intValue;
};

struct Command {
  CommandType type;
  Parameter parameters[MAX_PARAMETERS];
  int numParams;
};

#endif