#ifndef Errors_h
#define Errors_h

#include "Protocol.h"

inline void SerialSendError(int errorCode) {
    Serial.print((char)MessageType::Error);
    Serial.println(errorCode);
}

inline void SerialSendError(int errorCode, int arg1) {
    Serial.print((char)MessageType::Error);
    Serial.print(errorCode);
    Serial.print(" ");
    Serial.println(arg1);
}

// ERORRS
#define ERROR_CONTROLLER_CANNOT_CHANGE_STATE 0
#define ERROR_CONTROLLER_INVALID_STATE 1
#define ERROR_CONTROLLER_NO_ACTION_TO_PERFORM 2
#define ERROR_CONTROLLER_OPERATION_CANNOT_BE_PERFORMED_IN_CURRENT_STATE 4
#define ERROR_INVALID_PARAMETER_NAME 5
#define ERROR_COMMAND_TOO_LONG 6

static const char* errorTranslations[] = {
    "Cannot change state to running unless we are in Idle state",   // ERROR_CONTROLLER_CANNOT_CHANGE_STATE
    "Invalid controller state: ",                                   // ERROR_CONTROLLER_INVALID_STATE
    "No action to perform",                                         // ERROR_CONTROLLER_NO_ACTION_TO_PERFORM
    "Operation cannot be performed in current state",               // ERROR_CONTROLLER_OPERATION_CANNOT_BE_PERFORMED_IN_CURRENT_STATE
    "Invalid parameter name",                                       // ERROR_INVALID_PARAMETER_NAME
    "Command too long, discarding command",                         // ERROR_COMMAND_TOO_LONG
};

#endif