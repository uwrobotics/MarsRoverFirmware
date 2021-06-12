#include "LimitSwitch.h"

using namespace GPIO;

LimitSwitch::LimitSwitch(DigitalIn limitPin, bool ActiveHigh) : m_limitPin(limitPin), m_ActiveHigh(ActiveHigh) {}

bool LimitSwitch::isPressed() {
  if (m_limitPin.is_connected()) {
    return !m_ActiveHigh ? !m_limitPin.read() : m_limitPin.read();
  }
  return false;
}

LimitSwitch::operator bool() {
  return m_limitPin.isPressed();
}

LimitSwitch::isConnected(){
  return m_limitPin.is_connected();
}