#pragma once

#include "mbed.h"
/*

This is the note added to the "Create Limit Switch Class" issue on Github:

Currently we've just been using DigitalIn for limit switches.
The goal of this task is to create a limit switch driver to abstract away whether
    the switch is active high or active low.

*/
// Research on whether you should make the DigitalIn vairable a reference to the actual variable
// I added the code to configure the LimtiSwitch class in the CMakeLists.txt file in the gpio library
// Find out later if you need to configure the supported_build_configurations.yaml file
class LimitSwitch {
 public:
  LimitSwitch(DigitalIn limitPin, bool activeHigh = 1, bool activeLow = 1);

 private:
  DigitalIn m_limitPin;
  bool m_activeHigh;
  bool m_activeLow;
};