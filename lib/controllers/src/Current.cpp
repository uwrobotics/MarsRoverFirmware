#include "Current.h"

using namespace Controller;

bool Current::update() {
  if (shouldUpdate()) {
    float current = 0;
    if (m_currentSensor->read(current)) {
      m_actuator->setValue(m_pid->compute(m_sp.load(), current));
      return true;
    }
  }
  return false;
}
