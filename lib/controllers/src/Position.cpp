#include "Position.h"

using namespace Controller;

bool Position::update() {
  if (shouldStop()) {
    stop();
  } else {
    if (float angle = 0; m_encoder.getAngleDeg(angle)) {
      m_actuator.setValue(m_pid.compute(m_setpoint.load(), angle));
      return true;
    }
  }
  return false;
}

void Position::stop() {
  // set sp to current angle
  float angle = 0;
  m_encoder.getAngleDeg(angle); // TODO: Handle failures better
  m_setpoint.store(angle);
  m_actuator.setValue(0);
}