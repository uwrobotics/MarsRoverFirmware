#include "Position.h"

using namespace Controller;

bool Position::update() {
  if (shouldUpdate()) {
    float angle = 0;
    if (m_encoder->getAngleDeg(angle)) {
      m_actuator->setValue(m_pid->compute(m_sp.load(), angle));
      return true;
    }
  }
  return false;
}
