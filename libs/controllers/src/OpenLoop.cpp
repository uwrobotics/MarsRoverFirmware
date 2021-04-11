#include "OpenLoop.h"

using namespace Controller;

void OpenLoop::stop() {
  setSetPoint(0);
  m_actuator.setValue(0);
}

bool OpenLoop::reset() {
  /* TODO: Improve error reporting to differentiate between encoder and cs errors */
  stop();
  bool enc_rst_success = m_encoder.reset();
  bool cs_rst_success  = m_currentSensor.has_value() && m_currentSensor.value().get().reset();
  return enc_rst_success && cs_rst_success;
}

std::optional<std::reference_wrapper<PID::PID>> OpenLoop::getPID() {
  return std::nullopt;
}

bool OpenLoop::update() {
  /* Move the motor even if an encoder read fails, but still report it */
  bool enc_update_success = m_encoder.update();
  bool cs_update_success  = true;
  bool stop_required      = false;
  if (m_currentSensor.has_value()) {
    cs_update_success = m_currentSensor.value().get().update();
  }
  if (shouldStop()) {
    stop_required = true;
    stop();
  } else {
    m_actuator.setValue(m_setpoint.load());
  }
  return enc_update_success && cs_update_success && !stop_required;
}
