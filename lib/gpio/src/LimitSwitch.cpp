#include "LimitSwitch.h"

// 25th March 2021 - Feedback from Younes for limit switch task:
//      make sure you write a test app
// I copied this piece of code from the PwmIn.cpp file. Find out what the piece of code means
using namespace GPIO;

LimitSwitch::LimitSwitch(DigitalIn limitPin, bool ActiveHigh) : m_limitPin(limitPin), m_ActiveHigh(ActiveHigh) {}

// You are not writing to the Limit Switch so you only need to use the DigitalIn read function
// Tells you if the Limit Switch has been pressed or not

bool LimitSwitch::isPressed() {
  // Make sure Pin is connected before checking if it's an active high or active low
  if (limitPin.is_connected()) {
    if (!m_ActiveHigh) {
      return !limitPin.read();
    }
    return !limitPin.read();
  }
  return false;
}