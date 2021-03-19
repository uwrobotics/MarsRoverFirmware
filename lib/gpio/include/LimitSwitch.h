#pragma once

#include "mbed.h"
/*

This is the note added to the "Create Limit Switch Class" issue on Github:

Currently we've just been using DigitalIn for limit switches.
The goal of this task is to create a limit switch driver to abstract away whether
    the switch is active high or active low.

*/
// Find out later if you need to configure the supported_build_configurations.yaml file
class LimitSwitch {
 public:
  LimitSwitch(DigitalIn limitPin, bool ActiveHigh);
  // Tells you whether the LImit Switch has been pressed or not
  bool isPressed();
  // ActiveHighorLow is not  avery descriptive name so you should call it something like "ActiveHigh"
 private:
  DigitalIn m_limitPin;
  // Active High is 1 and Active Low is 0
  bool m_ActiveHigh;
};