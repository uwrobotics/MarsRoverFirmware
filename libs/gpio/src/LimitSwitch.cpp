#include "LimitSwitch.h"

using namespace GPIO;

LimitSwitch::LimitSwitch(DigitalIn limitPin, bool ActiveHigh) : m_limitPin(limitPin), m_ActiveHigh(ActiveHigh) {}

bool LimitSwitch::isPressed() {
  // Make sure Pin is connected before checking if it's an active high or active low
  if (bool(m_limitPin)) {
    if (!m_ActiveHigh) {
      return !m_limitPin.read();
    }
    return m_limitPin.read();
  }
  return false;
}

LimitSwitch::operator bool() {
  return m_limitPin.is_connected();
}