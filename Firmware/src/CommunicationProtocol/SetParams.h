#ifndef SetParams_h
#define SetParams_h

// NOTE! prefix 'I' is reserved for message ID field
enum class SetParamsParams {
    TelemetryInterval = 'E',
    PullerRPM = 'P',
    Temperature = 'T',
    FilamentSpoolDiameter = 'D',
    FilamentSpoolWidth = 'W',
    FilamentGuideStartPos = 'S',
    FilamentGuideStopPos = 'O',
    FilamentGuideArmBottomPosition = 'B',
    FilamentGuideArmTopPosition = 'A',
};

#endif