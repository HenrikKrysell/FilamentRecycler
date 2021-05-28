#ifndef PerformAction_h
#define PerformAction_h

#include "Protocol.h"

// NOTE! prefix 'I' is reserved for message ID field
enum class PerformActionSubType {
    None = 0,
    Production = 1,
    HomeAllAxes = 2,
    PreHeat = 3,
    MoveAxisRelative = 4,
    MoveAxisAbsolute = 5,
    Result = 6,
    UnwindSpool = 7,
};

// NOTE! prefix 'I' is reserved for message ID field
enum class ResultActionParams {
    Error = 'E',
    Stopped = 'T',
    Success = 'S'
};

// NOTE! prefix 'I' is reserved for message ID field
enum class MoveAxisActionParams {
    FilamentSpooler = 'S',
    FilamentGuide = 'G',
    Puller = 'P'
};

inline void sendActionResult(int id, ResultActionParams result) 
{
    Serial.print((char)MessageType::PerformAction);
    Serial.print((int)PerformActionSubType::Result);
    Serial.print(" ");
    Serial.print(ID_PARAMETER_PREFIX);
    Serial.print(id);
    Serial.print(" ");
    Serial.println((char)result);
}

#endif