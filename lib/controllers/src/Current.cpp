#include "Current.h"

using namespace Controller;

Current::Current(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
                 const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, PID::PID &pid,
                 float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit,
                 bool ignoreDegPerSecChecks, bool ignoreCurrentChecks, bool ignoreLimitSwitchChecks)
    : ActuatorController(actuator, encoder, currentSensor, maxDegPerSec, maxCurrent, lowerLimit, upperLimit,
                         ignoreDegPerSecChecks, ignoreCurrentChecks, ignoreLimitSwitchChecks),
      m_pid(pid) {}

bool Current::update() {
  if (shouldStop()) {
    stop();
  } else {
    float current = 0;
    if (m_currentSensor && m_currentSensor.value().get().read(current)) {
      m_actuator.setValue(m_pid.compute(m_setpoint.load(), current));
      return true;
    }
  }
  return false;
}

void Current::stop() {
  setSetPoint(0);
  m_actuator.setValue(0);
}

void Current::reset() {
  stop();
  m_encoder.reset();
  if (m_currentSensor.has_value()) {
    m_currentSensor.value().get().reset();
  }
  m_pid.reset();
}

std::optional<std::reference_wrapper<PID::PID>> Current::getPID() {
  return m_pid;
}
