#include "LimitSwitch.h"

// I copied this piece of code from the PwmIn.cpp file. Find out what the piece of code means
using namespace GPIO;

LimitSwitch::LimitSwitch(DigitalIn limitPin, bool ActiveHigh) : m_limitPin(limitPin), m_ActiveHigh(ActiveHigh) {}

// Instead of having the setActiveHighorLow() function, you should have a function like "isPressed()
// or any other descriptive name that just tells you if the LImit Switch ahs been pressed"
// You are not writing to the Limit Switch so you only need to use the DigitalIn read function
bool LimitSwitch::isPressed() {
  // Tells you if the Limit Switch has been pressed or not
  if (!m_ActiveHigh) {
    return !limitPin.read();
  }
  return !limitPin.read();
}