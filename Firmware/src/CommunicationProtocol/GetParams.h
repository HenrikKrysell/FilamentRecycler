#ifndef GetParams_h
#define GetParams_h

// NOTE! prefix 'I' is reserved for message ID field
enum class GetParamsParams {
    ControllerState = 'C',
    SpoolWinderState = 'S',
    FilamentExtruderState = 'E',
    PullerRPM = 'P',
    ExtruderRPM = 'X',
    Temperature = 'T',
    FilamentGuidePos = 'G',     
    ErrorCodes = 'R',
};

#endif