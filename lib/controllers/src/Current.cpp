#include "Current.h"

using namespace Controller;

bool Current::update() {
  if (shouldStop()) {
    stop();
  } else {
    float current = 0;
    if (m_currentSensor && m_currentSensor.value().get().read(current)) {
      m_actuator.setValue(m_pid.compute(m_setpoint.load(), current));
      return true;
    }
  }
  return false;
}

void Current::stop() {
  setSetPoint(0);
  m_actuator.setValue(0);
}
