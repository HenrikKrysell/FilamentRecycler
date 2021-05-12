#ifndef Command_h
#define Command_h

#define MAX_PARAMETERS 10

enum CommandType {
  Nothing = 0,
  ChangeState = 1,
  SetSpeed = 2,
  MoveAxis = 3,
  SetTemperature = 4,
  CommandTypeEnd,
};

static const char* CommandExplanation[] = {
  "0:Nothing - Does nothing",
  "1:ChangeState - Syntax: C1 SXX - Where X is the controller state number",
  "2:SetSpeed - Syntax: C2 WXX GXX PXX - Set the speed of W-winder motor, G-filament guide motor, P-filament puller motor",
  "3:MoveAxis - Syntax: C3 WXX GXX PXX - Moves the motors a specific amount of steps, W-winder motor, G-filament guide motor, P-filament puller motor",
  "4:SetTemperature - Syntax: C4 TXXX - Set target temperature to XXX",
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