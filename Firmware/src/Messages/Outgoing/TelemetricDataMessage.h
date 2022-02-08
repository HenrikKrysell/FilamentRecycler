#ifndef TelemetricDataMessage_h
#define TelemetricDataMessage_h

#include "MessageTypes.h"

struct TelemetricDataMessageHeader {
    byte messageType: 4;
    bool filamentGuideEndStopTriggered: 1;
    bool filamentGuideStepperMoving: 1; // true if the filament guide stepper still has steps to move to reach set position
    bool reserved1: 1;
    bool reserved2: 1;
} __attribute__((__packed__));

struct TelemetricDataMessage
{
    TelemetricDataMessageHeader header;
    unsigned int temperatureADC;
    byte rpmCount;
    unsigned int armPositionADC;
    unsigned int filamentDiameterADC;
} __attribute__((__packed__));

void sendTelemetricData(int temperature, byte rpmCount, int armPosition, int filamentDiameter, bool filamentGuideEndStopTriggered, bool filamentGuideStepperMoving) {
//   // TODO! Use 1-4 bits for the message type and add these flags to the first byte
  
//   // TODO! by using 10 bits per int instead of 16 we will save 2 bytes and go from 77 us per send to 50us
//   char buff[8] = {
//     OUTGOING_MESSAGE_TYPE::TELEMETRIC_DATA,
//     ((char*)&temperature)[0],
//     ((char*)&temperature)[1],
//     (char)rpmCount,
//     ((char*)&armPosition)[0],
//     ((char*)&armPosition)[1],
//     ((char*)&filamentDiameter)[0],
//     ((char*)&filamentDiameter)[1]
//   };
//  Serial.write(buff, 8);
    TelemetricDataMessage m;
    m.header.messageType = OUTGOING_MESSAGE_TYPE::TELEMETRIC_DATA;
    m.header.filamentGuideEndStopTriggered = filamentGuideEndStopTriggered;
    m.header.filamentGuideStepperMoving = filamentGuideStepperMoving;
    m.temperatureADC = temperature;
    m.rpmCount = rpmCount;
    m.armPositionADC = armPosition;
    m.filamentDiameterADC = filamentDiameter;

    Serial.write((char*)&m, sizeof(TelemetricDataMessage));
}

#endif