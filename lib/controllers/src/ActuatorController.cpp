#include "ActuatorController.h"

using namespace Controller;

ActuatorController::ActuatorController(
    Actuator::Actuator &actuator, Encoder::Encoder &encoder,
    const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, float maxDegPerSec,
    float maxCurrent, PinName lowerLimit, PinName upperLimit)
    : m_actuator(actuator),
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

void ActuatorController::overrideCurrentChecks() {
  m_ignoreCurrentChecks.store(true);
}

void ActuatorController::reinstateCurrentChecks() {
  m_ignoreCurrentChecks.store(false);
}

void ActuatorController::overrideDegPerSecChecks() {
  m_ignoreDegPerSecChecks.store(true);
}

void ActuatorController::reinstateDegPerSecChecks() {
  m_ignoreDegPerSecChecks.store(false);
}

void ActuatorController::overrideLimitSwitchChecks() {
  m_ignoreLimitSwitchChecks.store(true);
}

void ActuatorController::reinstateLimitSwitchChecks() {
  m_ignoreLimitSwitchChecks.store(false);
}

bool ActuatorController::reportAngleDeg(float &angle) {
  return m_encoder.getAngleDeg(angle);
}

bool ActuatorController::reportAngularVelocityDegPerSec(float &speed) {
  return m_encoder.getAngularVelocityDegPerSec(speed);
}

bool ActuatorController::shouldStop() {
  // this takes advantage of short-circuiting for faster evaluation
  float speed = 0, current = 0;
  bool shouldStop = !m_ignoreDegPerSecChecks.load() && m_encoder.getAngularVelocityDegPerSec(speed) &&
                    std::abs(speed) > m_maxDegPerSec;
  shouldStop = shouldStop || (!m_ignoreCurrentChecks.load() && m_currentSensor.has_value() &&
                              m_currentSensor.value().get().read(current) && std::abs(current) > m_maxCurrent);
  shouldStop = shouldStop || (!m_ignoreLimitSwitchChecks.load() && m_upperLimit.is_connected() && m_upperLimit.read() &&
                              m_setpoint.load() > 0);
  shouldStop = shouldStop || (!m_ignoreLimitSwitchChecks.load() && m_lowerLimit.is_connected() && m_lowerLimit.read() &&
                              m_setpoint.load() < 0);

  return shouldStop;
}
