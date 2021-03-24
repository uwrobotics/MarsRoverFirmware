#include "Position.h"

using namespace Controller;

Position::Position(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
                   const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor,
                   PID::PID &pid, float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit,
                   bool ignoreDegPerSecChecks, bool ignoreCurrentChecks, bool ignoreLimitSwitchChecks)
    : ActuatorController(actuator, encoder, currentSensor, maxDegPerSec, maxCurrent, lowerLimit, upperLimit,
                         ignoreDegPerSecChecks, ignoreCurrentChecks, ignoreLimitSwitchChecks),
      m_pid(pid) {}

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
  m_encoder.getAngleDeg(angle);  // TODO: Handle failures better
  m_setpoint.store(angle);
  m_actuator.setValue(0);
}

void Position::reset() {
  stop();
  m_encoder.reset();
  if (m_currentSensor.has_value()) {
    m_currentSensor.value().get().reset();
  }
  m_pid.reset();
}

std::optional<std::reference_wrapper<PID::PID>> Position::getPID() {
  return m_pid;
}