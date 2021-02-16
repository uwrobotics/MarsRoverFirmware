#include "Velocity.h"

using namespace Controller;

bool Velocity::update() {
  if (shouldStop()) {
    stop();
  } else {
    if (float speed = 0; m_encoder.getAngularVelocityDegPerSec(speed)) {
      m_actuator.setValue(m_pid.compute(m_setpoint.load(), speed));
      return true;
    }
  }
  return false;
}

void Velocity::stop() {
  setSetPoint(0);
  m_actuator.setValue(0);
}
