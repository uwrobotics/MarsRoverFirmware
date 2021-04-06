#pragma once

namespace GPIO {

class LimitSwitch {
 public:
  LimitSwitch(DigitalIn limitPin, bool ActiveHigh = 1);
  // Tells you whether the LImit Switch has been pressed or not
  bool isPressed();

  // Use operator overloading to overload bool operator

 private:
  DigitalIn m_limitPin;
  // Active High is 1 and Active Low is 0
  bool m_ActiveHigh;
};

}  // namespace GPIO