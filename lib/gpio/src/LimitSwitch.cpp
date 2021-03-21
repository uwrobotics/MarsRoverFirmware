#include "LimitSwitch.h"

// I copied this piece of code from the PwmIn.cpp file. Find out what the piece of code means
using namespace GPIO;

LimitSwitch::LimitSwitch(DigitalIn limitPin, bool ActiveHigh) : m_limitPin(limitPin), m_ActiveHigh(ActiveHigh) {}

// You are not writing to the Limit Switch so you only need to use the DigitalIn read function
// Tells you if the Limit Switch has been pressed or not
bool LimitSwitch::isPressed() {
  if (!m_ActiveHigh) {
    return !limitPin.read();
  }
  return !limitPin.read();
}