#include "src/OlimexMotorDriver.h"
#include "src/A4988StepperDriver.h"

struct AnlogInput
{
  /* data */
};

struct RPMCounter
{
  char pin;

  unsigned long count;  
};

struct InitMessage
{
  char messageType;

  char motorINAPin;
  char motorINBPin;
  char motorPWMPin;

  char motorRPMCounterPin;

  char pullerStepperEnablePin;
  char pullerStepperStepPin;
  char pullerStepperDirPin;
  char pullerStepperDirValue;

  char winderStepperEnablePin;
  char winderStepperStepPin;
  char winderStepperDirPin;
  char winderStepperDirValue;

  char thermistorAnalogPin;
  char heaterPin;

  char winderArmAnalogPin;
};

struct SetValuesMessage
{
  // turn on heater?
  byte messageType;
  byte augerMotorSpeed;
  byte winderStepperRPM;
  byte pullerStepperRPM;
  char filamentGuideSteps;
};

enum INCOMMING_MESSAGE_TYPE {
  INITIALIZE = 8,
  SET_VALUES = 9,
};

enum OUTGOING_MESSAGE_TYPE {
  WELCOME = 0,
  ERROR = 1,
  INITIALIZE_DONE = 2,
  TELEMETRIC_DATA = 3,
};

enum ERROR_CODE {
  INVALID_INITIALIZE_MESSAGE = 1,
  INVALID_MESSAGE_TYPE = 2,
};

enum class CONTROLLER_STATE {
  INITIALIZATION = 0,
  PRODUCTION = 1,
  CONFIGURATION = 2,
  IDLE = 3,
};

const int MESSAGE_SIZE = sizeof(SetValuesMessage);
const byte TERMINATOR = 0xff;
const int TELEMETRIC_DATA_INTERVAL_MS = 500;

A4988StepperDriver pullerStepperMotor;
A4988StepperDriver winderStepperMotor;
A4988StepperDriver filamentGuideStepperMotor;
OlimexMotorDriver augerMotor;
RPMCounter rpmCounter;
byte thermistorPin;
byte heaterPin;
byte winderArmAnalogPin;
unsigned long lastTelemetricTime;
CONTROLLER_STATE state;

void sendError(ERROR_CODE errorCode) {
  char buff[2] = {OUTGOING_MESSAGE_TYPE::ERROR, errorCode};
  Serial.write(buff, 2);
}

void sendMessage(OUTGOING_MESSAGE_TYPE msgType) {
  char buff[1] = {msgType};
  Serial.write(buff, 1);
}

void sendTelemetricData(int temperature, byte rpmCount, int armPosition, int filamentDiameter) {
  char buff[8] = {
    OUTGOING_MESSAGE_TYPE::TELEMETRIC_DATA,
    ((char*)&temperature)[0],
    ((char*)&temperature)[1],
    (char)rpmCount,
    ((char*)&armPosition)[0],
    ((char*)&armPosition)[1],
    ((char*)&filamentDiameter)[0],
    ((char*)&filamentDiameter)[1]};
  Serial.write(buff, 6);
}

bool waitForInitMessage() {  
  sendMessage(OUTGOING_MESSAGE_TYPE::WELCOME);

  char buffer[sizeof(InitMessage)];  
  size_t bytesRead = 0;
  while (bytesRead <= 0)
  {
    bytesRead = Serial.readBytesUntil(TERMINATOR, buffer, sizeof(InitMessage));
  }
  

  InitMessage *msg = (InitMessage*)&buffer;
  if (msg->messageType != INCOMMING_MESSAGE_TYPE::INITIALIZE) {
    sendError(ERROR_CODE::INVALID_INITIALIZE_MESSAGE);
    return false;
  }

  augerMotor.init(msg->motorINAPin, msg->motorINBPin, msg->motorPWMPin);

  rpmCounter.pin = msg->motorRPMCounterPin;

  pullerStepperMotor.init(msg->pullerStepperDirPin, msg->pullerStepperStepPin, msg->pullerStepperEnablePin);
  winderStepperMotor.init(msg->winderStepperDirPin, msg->winderStepperStepPin, msg->winderStepperEnablePin);

  thermistorPin = msg->thermistorAnalogPin;
  heaterPin = msg->heaterPin;
  winderArmAnalogPin = msg->winderArmAnalogPin;

  return true;
}

void setup() {
  Serial.begin(250000);

  while (!waitForInitMessage())
  {
    // Empty
  }

  pinMode(thermistorPin, INPUT);

  // DEBUG
  winderStepperMotor.setClockwiseRotation();
  pullerStepperMotor.setClockwiseRotation();

  augerMotor.setClockwiseRotation();
  augerMotor.setSpeed(0);
  lastTelemetricTime = 0;

  state = CONTROLLER_STATE::PRODUCTION;

  sendMessage(OUTGOING_MESSAGE_TYPE::INITIALIZE_DONE);
}

void handleMessage() {
  if (Serial.available() < MESSAGE_SIZE)
    return;

  byte buffer[MESSAGE_SIZE];
  Serial.readBytes(buffer, MESSAGE_SIZE);
  switch (buffer[0])
  {
  case INCOMMING_MESSAGE_TYPE::SET_VALUES:
    {
      SetValuesMessage *msg = (SetValuesMessage*)&buffer;
      augerMotor.setSpeed(msg->augerMotorSpeed);
      winderStepperMotor.setRPM(msg->winderStepperRPM);
      pullerStepperMotor.setRPM(msg->pullerStepperRPM);
    }
    break;
  
  default:
    sendError(ERROR_CODE::INVALID_MESSAGE_TYPE);
    break;
  }
}

void loop() {
  // switch (state)
  // {
  // case CONTROLLER_STATE::PRODUCTION:
  //   handleMessage();
  //   pullerStepperMotor.run();
  //   winderStepperMotor.run();

  //   if (millis() - lastTelemetricTime > TELEMETRIC_DATA_INTERVAL_MS) {
  //     lastTelemetricTime = millis();
  //     sendTelemetricData(1020, 25, 500, 1001);
  //   }
  //   break;
  // case CONTROLLER_STATE::INITIALIZATION:
  //   break;
  // case CONTROLLER_STATE::IDLE:
  //   break;
  // case CONTROLLER_STATE::CONFIGURATION:
  //   break;
  // default:
  //   break;
  // }

  ///////////////////////////////////////
  // analogRead test
  unsigned long m1 = micros();
  for (size_t i = 0; i < 1000; i++)
  {
    int r = analogRead(thermistorPin);
    r = analogRead(thermistorPin);
    r = analogRead(thermistorPin);
  }
  unsigned long m2 = micros();
  Serial.print("'");
  Serial.print("analog read: ");
  Serial.print((m2 - m1));
  Serial.print("'");
  ///////////////////////////////////////


  ///////////////////////////////////////
  // Switch performance test
  // int sw = 0;
  // unsigned long m1 = micros();
  // for (size_t i = 0; i < 20000; i++)
  // {
  //   switch (sw)
  //   {
  //   case 0:
  //     sw = 1;
  //     break;
  //   case 1:
  //     sw = 2;
  //     break;
  //   case 2:
  //     sw = 3;
  //     break;
  //   case 3:
  //     sw = 4;
  //     break;
  //   case 4:
  //     sw = 0;
  //     break;
    
  //   default:
  //     break;
  //   }
  // }
  // unsigned long m2 = micros();
  // Serial.print("'");
  // Serial.print("Switch: ");
  // Serial.print((m2 - m1));
  // Serial.print("'");

  // m1 = micros();
  // for (size_t i = 0; i < 20000; i++)
  // {
  //   sw += 1;
  // }
  // m2 = micros();
  // Serial.print("'");
  // Serial.print("Addition: ");
  // Serial.print((m2 - m1));
  // Serial.print("'");
  // Serial.print("'");
  // Serial.print("sw: ");
  // Serial.print(sw);
  // Serial.print("'");
  // delay(600000);
  ///////////////////////////////////////

  ///////////////////////////////////////
  // Read test
  // char buffer[sizeof(InitMessage)];  
  // unsigned long m1 = micros();
  // for (size_t i = 0; i < 1000; i++)
  // {
  //   int avilable = Serial.available();
  //   avilable = Serial.available();
  //   avilable = Serial.available();
  // }
  // unsigned long m2 = micros();
  // Serial.print("'");
  // Serial.print((m2 - m1));
  // Serial.print("'");
  // delay(600000);
  ///////////////////////////////////////
  

  ///////////////////////////////////////
  // Send test
  // unsigned long sendMicros = 0;
  // for (size_t i = 0; i < 100; i++)
  // {
  //   unsigned long m1 = micros();
  //   sendTelemetricData(1020, 25, 500, 1001);
  //   unsigned long m2 = micros();
  //   sendMicros += m2 - m1;
  //   delay(100);
  // }
  // Serial.print("'");
  // Serial.print(sendMicros / 100);   // 70 us for 8 bytes 48 us for 6 bytes
  // Serial.print("'");
  // delay(600000);
  ///////////////////////////////////////
}
