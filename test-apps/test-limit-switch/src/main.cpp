#include "LimitSwitch.h"
#include "PinNames.h"
#include "mbed.h"

GPIO::LimitSwitch hSwitch(PA_0);
GPIO::LimitSwitch lSwitch(PA_0, 0);
GPIO::LimitSwitch ncSwitch(NC);

int main() {
  // Check to see if limit switch is pressed
  while (true) {
    // check for active high and active low case
    if (hSwitch.isPressed()) {
      printf("Active high limit switch is pressed \n");
    }

    if (lSwitch.isPressed()) {
      printf("Active low limit switch is pressed \n");
    }
    // test for NC limit switch
    if (!ncSwitch.isConnected()) {
      printf("Pin is not connected \n");
    }
  }
}