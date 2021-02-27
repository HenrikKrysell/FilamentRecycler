#include <AccelStepper.h>
#include "SpoolWinder.h"

SpoolWinder spoolWinder = SpoolWinder(25, 23, 27, 24, 22);

void setup() {
  spoolWinder.setup();
}

void loop() {
  spoolWinder.loop();
}
