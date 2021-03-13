#include "OpenLoop.h"

using namespace Controller;

void OpenLoop::stop() {
  setSetPoint(0);
  m_actuator.setValue(0);
}

void OpenLoop::reset() {
  stop();
  m_encoder.reset();
  if (m_currentSensor.has_value()) {
    m_currentSensor.value().get().reset();
  }
}

std::optional<std::reference_wrapper<PID::PID>> OpenLoop::getPID() {
  return std::nullopt;
}

bool OpenLoop::update() {
  shouldStop() ? stop() : m_actuator.setValue(m_setpoint.load());
  return true;
}
