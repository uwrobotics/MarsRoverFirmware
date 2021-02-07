#include "Position.h"

using namespace Controller;

void Position::update() {
  if (!m_ignoreRPMChecks.load()) {
    if (std::abs(m_encoder->getAngularVelocityDegPerSec()) > m_maxDegPerSec) {
      stop();
    }
  }
  if (!m_ignoreCurrentChecks.load()) {
    if (std::abs(m_encoder->read()) > m_maxDegPerSec) {
      stop();
    }
  }
  if (m_upperLimit) {
    if (m_upperLimit.value()->read() && m_sp.load() > 0) {
      return;
    }
  }
  if (m_lowerLimit) {
    if (m_lowerLimit.value()->read() && m_sp.load() < 0) {
      return;
    }
  }
  m_actuator->setValue(pid->compute(m_sp.load(), m_encoder->getAngleDeg()));
}
