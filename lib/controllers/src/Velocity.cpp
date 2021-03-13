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

void Velocity::reset() {
  stop();
  m_encoder.reset();
  if (m_currentSensor.has_value()) {
    m_currentSensor.value().get().reset();
  }
  m_pid.reset();
}

std::optional<std::reference_wrapper<PID::PID>> Velocity::getPID() {
  return m_pid;
}