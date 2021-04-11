#include "Current.h"

using namespace Controller;

Current::Current(Actuator::Actuator &actuator, Encoder::Encoder &encoder, Sensor::CurrentSensor &currentSensor,
                 PID::PID &pid, float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit,
                 bool ignoreDegPerSecChecks, bool ignoreCurrentChecks, bool ignoreLimitSwitchChecks)
    : ActuatorController(actuator, encoder, currentSensor, maxDegPerSec, maxCurrent, lowerLimit, upperLimit,
                         ignoreDegPerSecChecks, ignoreCurrentChecks, ignoreLimitSwitchChecks),
      m_pid(pid) {}

bool Current::update() {
  /* Proceed even if encoder read fails, but report */
  bool enc_update_success = m_encoder.update();
  bool cs_update_success  = m_currentSensor.value().get().update();
  bool stop_required      = false;
  if (shouldStop()) {
    stop();
    stop_required = true;
  } else {
    m_actuator.setValue(m_pid.compute(m_setpoint.load(), m_currentSensor.value().get().read()));
  }
  return enc_update_success && cs_update_success && !stop_required;
}

void Current::stop() {
  setSetPoint(0);
  m_actuator.setValue(0);
}

bool Current::reset() {
  stop();
  m_pid.reset();
  bool enc_rst_success = m_encoder.reset();
  bool cs_rst_success  = m_currentSensor.value().get().reset();
  return enc_rst_success && cs_rst_success;
}

std::optional<std::reference_wrapper<PID::PID>> Current::getPID() {
  return m_pid;
}
