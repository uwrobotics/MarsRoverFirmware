#include "LimitSwitch.h"

using namespace GPIO;

LimitSwitch::LimitSwitch(DigitalIn limitPin, bool ActiveHigh) : m_limitPin(limitPin), m_ActiveHigh(ActiveHigh) {}

bool LimitSwitch::isPressed() {
  // Make sure Pin is connected before checking if it's an active high or active low
  if (limitPin.is_connected()) {
    if (!m_ActiveHigh) {
      return !limitPin.read();
    }
    return limitPin.read();
  }
  return false;
}