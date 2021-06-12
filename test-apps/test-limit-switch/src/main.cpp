#include "LimitSwitch.h"
#include "PinNames.h"
#include "mbed.h"

DigitalIn Pin(PA_0);
GPIO::LimitSwitch hSwitch(Pin);
GPIO::LimitSwitch lSwitch(Pin, 0);
GPIO::LimitSwitch ncSwitch(NC); 

int main() {
  // Check to see if limit switch is pressed
  while (true) {
    if (hSwitch.isPressed()) {
      printf("Limit switch is pressed \n");
    }

    // check for active high and active low case
    if (hSwitch.isPressed()) {
      printf("Active high limit switch is pressed \n");
    }

    if (lSwitch.isPressed()) {
      printf("Active low limit switch is pressed \n");
    }
    // test for NC limit switch
    while (!ncSwitch.isConnected()) {
      printf("Pin is not conneceted \n");
    }
  }
}