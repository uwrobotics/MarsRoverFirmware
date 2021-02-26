#include "BasicControl.h"

using namespace Controller;

BasicControl::BasicControl(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
                           const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor,
                           PID::PID &pid, float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit)
    : m_actuator(actuator),
      m_encoder(encoder),
      m_currentSensor(currentSensor),
      m_pid(pid),
      m_maxDegPerSec(maxDegPerSec),
      m_maxCurrent(maxCurrent),
      m_lowerLimit(lowerLimit),
      m_upperLimit(upperLimit) {}

void BasicControl::reset() {
  stop();
  m_encoder.reset();
  if (m_currentSensor) {
    m_currentSensor.value().get().reset();
  }
  m_pid.reset();
}

std::optional<std::reference_wrapper<PID::PID>> BasicControl::getPID() {
  return m_pid;
}

bool BasicControl::reportAngleDeg(float &angle) {
  return m_encoder.getAngleDeg(angle);
}

bool BasicControl::reportAngularVelocityDegPerSec(float &speed) {
  return m_encoder.getAngularVelocityDegPerSec(speed);
}

bool BasicControl::shouldStop() {
  // this takes advantage of short-circuiting for faster evaluation
  float speed = 0, current = 0;
  bool shouldStop = !m_ignoreDegPerSecChecks.load() && m_encoder.getAngularVelocityDegPerSec(speed) &&
                    std::abs(speed) > m_maxDegPerSec;
  shouldStop = shouldStop || (!m_ignoreCurrentChecks.load() && m_currentSensor != std::nullopt &&
                              m_currentSensor.value().get().read(current) && std::abs(current) > m_maxCurrent);
  shouldStop = shouldStop || (m_upperLimit.is_connected() && m_upperLimit.read() && m_setpoint.load() > 0);
  shouldStop = shouldStop || (m_lowerLimit.is_connected() && m_lowerLimit.read() && m_setpoint.load() < 0);

  return shouldStop;
}
