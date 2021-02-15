#include "Velocity.h"

using namespace Controller;

bool Velocity::update() {
  if (shouldUpdate()) {
    float speed = 0;
    if (m_encoder.getAngularVelocityDegPerSec(speed)) {
      m_actuator.setValue(m_pid.compute(m_setpoint.load(), speed));
      return true;
    }
  }
  return false;
}
