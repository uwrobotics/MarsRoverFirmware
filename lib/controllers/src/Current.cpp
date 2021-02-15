#include "Current.h"

using namespace Controller;

bool Current::update() {
  if (shouldUpdate()) {
    float current = 0;
    if (m_currentSensor) {
      if (m_currentSensor.value().get().read(current)) {
        m_actuator.setValue(m_pid.compute(m_setpoint.load(), current));
        return true;
      }
    }
  }
  return false;
}
