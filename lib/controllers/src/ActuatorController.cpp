#include "ActuatorController.h"

using namespace Controller;

ActuatorController::ActuatorController(
    Actuator::Actuator &actuator, Encoder::Encoder &encoder,
    const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, float maxDegPerSec,
    float maxCurrent, PinName lowerLimit, PinName upperLimit, bool ignoreDegPerSecChecks, bool ignoreCurrentChecks,
    bool ignoreLimitSwitchChecks)
    : m_ignoreDegPerSecChecks(ignoreDegPerSecChecks),
      m_ignoreCurrentChecks(ignoreCurrentChecks),
      m_ignoreLimitSwitchChecks(ignoreLimitSwitchChecks),
      m_actuator(actuator),
      m_encoder(encoder),
      m_currentSensor(currentSensor),
      m_maxDegPerSec(maxDegPerSec),
      m_maxCurrent(maxCurrent),
      m_lowerLimit(lowerLimit),
      m_upperLimit(upperLimit) {}

void ActuatorController::setSetPoint(float sp) {
  m_setpoint.store(sp);
}

float ActuatorController::getSetPoint() const {
  return m_setpoint.load();
}

void ActuatorController::deactivateCurrentChecks() {
  m_ignoreCurrentChecks.store(true);
}

void ActuatorController::activateCurrentChecks() {
  m_ignoreCurrentChecks.store(false);
}

void ActuatorController::deactivateDegPerSecChecks() {
  m_ignoreDegPerSecChecks.store(true);
}

void ActuatorController::activateDegPerSecChecks() {
  m_ignoreDegPerSecChecks.store(false);
}

void ActuatorController::deactivateLimitSwitchChecks() {
  m_ignoreLimitSwitchChecks.store(true);
}

void ActuatorController::activateLimitSwitchChecks() {
  m_ignoreLimitSwitchChecks.store(false);
}

float ActuatorController::reportAngleDeg() {
  return m_encoder.getAngleDeg();
}

float ActuatorController::reportAngularVelocityDegPerSec() {
  return m_encoder.getAngularVelocityDegPerSec();
}

bool ActuatorController::shouldStop() {
  // this takes advantage of short-circuiting for faster evaluation
  bool shouldStop =
      !m_ignoreDegPerSecChecks.load() && std::abs(m_encoder.getAngularVelocityDegPerSec()) > m_maxDegPerSec;
  shouldStop = shouldStop || (!m_ignoreCurrentChecks.load() && m_currentSensor.has_value() &&
                              std::abs(m_currentSensor.value().get().read()) > m_maxCurrent);
  shouldStop = shouldStop || (!m_ignoreLimitSwitchChecks.load() && m_upperLimit.is_connected() && m_upperLimit.read() &&
                              m_setpoint.load() > 0);
  shouldStop = shouldStop || (!m_ignoreLimitSwitchChecks.load() && m_lowerLimit.is_connected() && m_lowerLimit.read() &&
                              m_setpoint.load() < 0);

  return shouldStop;
}
