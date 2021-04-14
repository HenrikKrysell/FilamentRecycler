#include "src/Controller.h"
#include "src/DataStructures/motorDefinition.h"

Controller controller = Controller();

void setup() {
  controller.setup();
  controller.changeState(ControllerStates::Homing);
}

void loop() {
  controller.loop();
}
