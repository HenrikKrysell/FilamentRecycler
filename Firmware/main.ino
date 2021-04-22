#include "src/Controller.h"
#include "src/DataStructures/motorDefinition.h"

Controller controller = Controller();

void setup() {
  Serial.begin(115200);

  controller.setup();
}

void loop() {
  controller.loop();
}
