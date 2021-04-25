#pragma once

namespace GPIO {

class LimitSwitch {
 public:
  LimitSwitch(DigitalIn limitPin, bool ActiveHigh = 1);
  // Tells you whether the Limit Switch has been pressed or not
  bool isPressed();

  // Overloaded bool operator
  bool operator bool();

 private:
  DigitalIn m_limitPin;
  // Active High is 1 and Active Low is 0
  bool m_ActiveHigh;
};

}  // namespace GPIO