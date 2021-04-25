#include "LimitSwitch.h"
#include "mbed.h"

LimitSwitch hSwitch(Pin);
LimitSwitch lSWitch(Pin, 0);
int main() {
  // Check to see if limit switch is pressed
  // check logger library to use cout functionaility
  while (true) {
    if (hSwitch.isPressed()) {
      std::cout >> "Limit switch is pressed/n";
    }
  }
  // check for active high and active low case
  while (true) {
    if (hSwitch.isPressed()) {
    }

    if (lSWitch.isPressed()) {
    }
  }
  // test for NC pin
  while (!hswitch.limitPin.is_connected()) {
  }
}