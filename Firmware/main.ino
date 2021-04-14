#include "src/SpoolWinder.h"
#include "src/DataStructures/motorDefinition.h"

SpoolWinder spoolWinder = SpoolWinder({.motorDirPin=25, .motorStepPin=23, .motorEnablePin=27}, {.motorDirPin=25, .motorStepPin=23, .motorEnablePin=27}, 10);

void setup() {
  spoolWinder.setup();
}

void loop() {
  spoolWinder.loop();
}
