#include "mbed.h"
#include "PinNames.h"
#include "LimitSwitch.h"

DigitalIn Pin(PA_0);
GPIO::LimitSwitch hSwitch(Pin);
GPIO::LimitSwitch lSwitch(Pin, 0);

int main() {
  // Check to see if limit switch is pressed
  while (true) {
    if (hSwitch.isPressed()) {
     printf("Limit switch is pressed \n");
    }
  }
  // check for active high and active low case
  while (true) {
    if (hSwitch.isPressed()) {
      printf("Active high limit switch is pressed \n");
    }

    if (lSwitch.isPressed()) {
      printf("Active low limit switch is pressed \n");
    }
  }
  // test for NC pin
  while (!hSwitch.m_limitPin.is_connected()) {
     printf("Pin is not conneceted \n");
  }
}