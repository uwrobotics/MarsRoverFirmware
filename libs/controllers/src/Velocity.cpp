#include "Velocity.h"

using namespace Controller;

Velocity::Velocity(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
                   const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor,
                   PID::PID &pid, float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit,
                   bool ignoreDegPerSecChecks, bool ignoreCurrentChecks, bool ignoreLimitSwitchChecks)
    : ActuatorController(actuator, encoder, currentSensor, maxDegPerSec, maxCurrent, lowerLimit, upperLimit,
                         ignoreDegPerSecChecks, ignoreCurrentChecks, ignoreLimitSwitchChecks),
      m_pid(pid) {}

bool Velocity::update() {
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
    m_actuator.setValue(m_pid.compute(m_setpoint.load(), m_encoder.getAngularVelocityDegPerSec()));
  }
  return enc_update_success && cs_update_success && !stop_required;
}

void Velocity::stop() {
  setSetPoint(0);
  m_actuator.setValue(0);
}

bool Velocity::reset() {
  stop();
  m_pid.reset();
  bool enc_rst_success = m_encoder.reset();
  bool cs_rst_success  = m_currentSensor.value().get().reset();
  return enc_rst_success && cs_rst_success;
}

std::optional<std::reference_wrapper<PID::PID>> Velocity::getPID() {
  return m_pid;
}