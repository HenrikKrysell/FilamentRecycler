#include "src/OlimexMotorDriver.h"
#include "src/A4988StepperDriver.h"
#include "src/Messages/Incomming/InitMessage.h"
#include "src/Messages/Incomming/SetValuesMessage.h"
#include "src/Messages/Incomming/MessageTypes.h"
#include "src/Messages/Outgoing/MessageTypes.h"
#include "src/Messages/Outgoing/Utils.h"
#include "src/Messages/Outgoing/TelemetricDataMessage.h"
#include "src/AnalogSignalPoller.h"
#include "src/DigitalSignalFlankCounter.h"

enum class CONTROLLER_STATE {
  INITIALIZATION = 0,
  PRODUCTION = 1,
  CONFIGURATION = 2,
  IDLE = 3,
  EMERGENCY_STOP = 4,
};

const int MESSAGE_SIZE = sizeof(SetValuesMessage);
const byte TERMINATOR = 0xff;


A4988StepperDriver pullerStepperMotor;
A4988StepperDriver winderStepperMotor;
A4988StepperDriver filamentGuideStepperMotor;
OlimexMotorDriver augerMotor;
DigitalSignalFlankCounter rpmCounter;
byte heaterPin;
unsigned long lastTelemetricTime;
CONTROLLER_STATE state;
AnalogSignalPoller thermistorPoller;
AnalogSignalPoller winderArmPoller;
AnalogSignalPoller filamentDiameterPoller;
int telemetricDataIntervalMs;

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

  ////////////////
  // DEBUG
  // Serial.write(DEBUG_MESSAGE_START);
  // Serial.println("InitMessage: ");
  // Serial.print("heaterPin: ");
  // Serial.println(msg->heaterPin);
  // Serial.print("motorDriver.inAPin: ");
  // Serial.println(msg->motorDriver.inAPin);
  // Serial.print("motorRPMCounter.pollIntervalMs: ");
  // Serial.println(msg->motorRPMCounter.pollIntervalMs);
  // Serial.print("pullerStepper.stepPin: ");
  // Serial.println(msg->pullerStepper.stepPin);
  // Serial.print("filamentGuideStepper.dirPin: ");
  // Serial.println(msg->filamentGuideStepper.dirPin);
  // Serial.print("telemetricDataIntervalMs: ");
  // Serial.println(msg->telemetricDataIntervalMs);
  // Serial.print("thermistorConfig.numSamples: ");
  // Serial.println(msg->thermistorConfig.numSamples);
  // Serial.print("filamentDiamterConfig.pin: ");
  // Serial.println(msg->filamentDiamterConfig.pin);
  // Serial.print("'");
  ////////////////

  augerMotor.init(msg->motorDriver.inAPin, msg->motorDriver.inBPin, msg->motorDriver.pwmPin);
  augerMotor.setSpeed(0);
  augerMotor.setRotation((MotorRotationDirection)msg->motorDriver.dir);

  rpmCounter.init(msg->motorRPMCounter.pin, msg->motorRPMCounter.pollIntervalMs, msg->motorRPMCounter.delayBetweenFirstPoll);

  pullerStepperMotor.init(msg->pullerStepper.dirPin, msg->pullerStepper.stepPin, msg->pullerStepper.enablePin);
  pullerStepperMotor.setRotation((MotorRotationDirection)msg->pullerStepper.dirValue);
  
  winderStepperMotor.init(msg->winderStepper.dirPin, msg->winderStepper.stepPin, msg->winderStepper.enablePin);
  winderStepperMotor.setRotation((MotorRotationDirection)msg->winderStepper.dirValue);

  filamentGuideStepperMotor.init(msg->filamentGuideStepper.dirPin, msg->filamentGuideStepper.stepPin, msg->filamentGuideStepper.enablePin);
  filamentGuideStepperMotor.setRotation((MotorRotationDirection)msg->filamentGuideStepper.dirValue);

  // // TODO! calculate a good delay between each analog poller to not choke the cpu
  // // this hardcoded value is only for a poll intervall of 500ms
  // thermistorPoller.init(msg->thermistorAnalogPin, msg->thermistorPollIntervalMs, 125, 8, 1);
  // winderArmPoller.init(msg->winderArmAnalogPin, msg->winderArmPollIntervalMs, 250, 1, 1);
  // filamentDiameterPoller.init(msg->filamentDiameterAnalogPin, msg->filamentDiameterPollIntervalMs, 375, 1, 1);
  thermistorPoller.init(
    msg->thermistorConfig.pin,
    msg->thermistorConfig.pollIntervalMs,
    msg->thermistorConfig.delayBetweenFirstPoll,
    msg->thermistorConfig.numSamples,
    msg->thermistorConfig.delayBetweenSamples
  );
  winderArmPoller.init(
    msg->winderArmConfig.pin,
    msg->winderArmConfig.pollIntervalMs,
    msg->winderArmConfig.delayBetweenFirstPoll,
    msg->winderArmConfig.numSamples,
    msg->winderArmConfig.delayBetweenSamples
  );
  filamentDiameterPoller.init(
    msg->filamentDiamterConfig.pin,
    msg->filamentDiamterConfig.pollIntervalMs,
    msg->filamentDiamterConfig.delayBetweenFirstPoll,
    msg->filamentDiamterConfig.numSamples,
    msg->filamentDiamterConfig.delayBetweenSamples
  );

  telemetricDataIntervalMs = msg->telemetricDataIntervalMs;  
  lastTelemetricTime = millis();

  heaterPin = msg->heaterPin;
  pinMode(heaterPin, OUTPUT);

  return true;
}

void setup() {
  Serial.begin(250000);

  while (!waitForInitMessage())
  {
    // Empty
  }

  state = CONTROLLER_STATE::PRODUCTION;
  //state = CONTROLLER_STATE::INITIALIZATION;

  sendMessage(OUTGOING_MESSAGE_TYPE::INITIALIZE_DONE);
}

void handleMessage() {
  if (Serial.available() < MESSAGE_SIZE)
    return;

  byte buffer[MESSAGE_SIZE];
  Serial.readBytes(buffer, MESSAGE_SIZE);
  MessageHeader *header = (MessageHeader*)&buffer[0];

  switch (header->messageType)
  {
  case INCOMMING_MESSAGE_TYPE::SET_VALUES:
    {
      SetValuesMessage *msg = (SetValuesMessage*)&buffer;
      augerMotor.setSpeed(msg->augerMotorSpeed);
      winderStepperMotor.setRPM(msg->winderStepperRPM);
      pullerStepperMotor.setRPM(msg->pullerStepperRPM);

      digitalWrite(heaterPin, msg->messageHeader.turnOnHeater);
      state = msg->messageHeader.emergencyStop ? CONTROLLER_STATE::EMERGENCY_STOP : state;
    }
    break;
  
  default:
    sendError(ERROR_CODE::INVALID_MESSAGE_TYPE);
    break;
  }
}

void loop() {
  switch (state)
  {
  case CONTROLLER_STATE::PRODUCTION:
    handleMessage();

    pullerStepperMotor.run();
    winderStepperMotor.run();
    filamentGuideStepperMotor.run();

    thermistorPoller.run();
    winderArmPoller.run();
    filamentDiameterPoller.run();

    rpmCounter.run();

    if (millis() - lastTelemetricTime > telemetricDataIntervalMs) {
      lastTelemetricTime = millis();
      sendTelemetricData(
        thermistorPoller.getValue(), 
        rpmCounter.getCount(), 
        winderArmPoller.getValue(), 
        filamentDiameterPoller.getValue(),
        true,
        false
      );
      rpmCounter.resetCount();
    }
    break;
  case CONTROLLER_STATE::INITIALIZATION:
    break;
  case CONTROLLER_STATE::IDLE:
    break;
  case CONTROLLER_STATE::CONFIGURATION:
    break;
  case CONTROLLER_STATE::EMERGENCY_STOP:
    digitalWrite(heaterPin, 0);
    break;
  default:
    break;
  }

  ///////////////////////////////////////
  // digitalRead test
  // unsigned long n1 = micros();
  // for (size_t i = 0; i < 1000; i++)
  // {
  //   int r = digitalRead(thermistorPin);
  //   r = digitalRead(thermistorPin);
  //   r = digitalRead(thermistorPin);
  // }
  // unsigned long n2 = micros();
  // Serial.print("'");
  // Serial.print("digital read: "); // 4 us
  // Serial.print((n2 - n1));
  // Serial.print("'");
  ///////////////////////////////////////

  ///////////////////////////////////////
  // analogRead test
  // unsigned long m1 = micros();
  // for (size_t i = 0; i < 1000; i++)
  // {
  //   int r = analogRead(thermistorPin);
  //   r = analogRead(thermistorPin);
  //   r = analogRead(thermistorPin);
  // }
  // unsigned long m2 = micros();
  // Serial.print("'");
  // Serial.print("analog read: "); // 112 us
  // Serial.print((m2 - m1));
  // Serial.print("'");
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
