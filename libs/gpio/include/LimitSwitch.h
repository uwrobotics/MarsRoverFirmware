#pragma once
#include "mbed.h"
namespace GPIO {

class LimitSwitch {
 public:
  LimitSwitch(DigitalIn limitPin, bool ActiveHigh = 1);
  // Tells you whether the Limit Switch has been pressed or not
  bool isPressed();

  // Overloaded bool operator
  operator bool();

  bool isConnected();

 private:
  // Active High is 1 and Active Low is 0
  bool m_ActiveHigh;
  DigitalIn m_limitPin;
};

}  // namespace GPIO