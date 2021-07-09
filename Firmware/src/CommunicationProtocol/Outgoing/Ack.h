#ifndef Ack_h
#define Ack_h

#include "OutgoingMessage.h"

// NOTE! prefix 'I' is reserved for message ID field
enum class AckResultParams {
    Error = 'E',
    Stopped = 'T',
    Success = 'S'
};

inline void sendAck(int id, AckResultParams resultType, int resultCode=-100) 
{
    Serial.print((char)OutgoingMessageType::Ack);
    Serial.print(" ");
    Serial.print(ID_PARAMETER_PREFIX);
    Serial.print(id);
    Serial.print(" ");
    Serial.print((char)resultType);
    if (resultCode == -100)
        Serial.println("");
    else
        Serial.println(resultCode);
}

inline void sendAck(AckResultParams resultType, int resultCode=-100) 
{
    Serial.print((char)OutgoingMessageType::Ack);
    Serial.print(" ");
    Serial.print((char)resultType);
    if (resultCode == -100)
        Serial.println("");
    else
        Serial.println(resultCode);
}

#endif