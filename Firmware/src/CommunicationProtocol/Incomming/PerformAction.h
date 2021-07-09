#ifndef PerformAction_h
#define PerformAction_h

#include "IncommingMessage.h"

// NOTE! prefix 'I' is reserved for message ID field
enum class PerformActionSubType {
    None = 0,
    Production = 1,
    HomeAllAxes = 2,
    PreHeat = 3,
    MoveAxisRelative = 4,
    MoveAxisAbsolute = 5,
    UnwindSpool = 7,
};

// NOTE! prefix 'I' is reserved for message ID field
enum class MoveAxisActionParams {
    FilamentSpooler = 'S',
    FilamentGuide = 'G',
    Puller = 'P'
};

#endif