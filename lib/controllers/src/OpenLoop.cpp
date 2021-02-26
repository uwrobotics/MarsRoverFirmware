#include "OpenLoop.h"
#include "../../utility/include/Logger.h"

using namespace Controller;

OpenLoop::OpenLoop(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
                   const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor,
                   float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit)
    : m_actuator(actuator),
      m_encoder(encoder),
      m_currentSensor(currentSensor),
      m_maxDegPerSec(maxDegPerSec),
      m_maxCurrent(maxCurrent),
      m_lowerLimit(lowerLimit),
      m_upperLimit(upperLimit) {}

void OpenLoop::stop() {
  Utility::Logger::printf("OpenLoop::stop() called\r\n");
  setSetPoint(0);
  m_actuator.setValue(0);
}

void OpenLoop::reset() {
  stop();
  m_encoder.reset();
  if (m_currentSensor) {
    m_currentSensor.value().get().reset();
  }
}

bool OpenLoop::reportAngleDeg(float &angle) {
  return m_encoder.getAngleDeg(angle);
}

bool OpenLoop::reportAngularVelocityDegPerSec(float &speed) {
  return m_encoder.getAngularVelocityDegPerSec(speed);
}

std::optional<std::reference_wrapper<PID::PID>> OpenLoop::getPID() {
  return std::nullopt;
}

bool OpenLoop::shouldStop() {
  // this takes advantage of short-circuiting for faster evaluation
  float speed = 0, current = 0;
  bool shouldStop = !m_ignoreDegPerSecChecks.load() && m_encoder.getAngularVelocityDegPerSec(speed) &&
                    std::abs(speed) > m_maxDegPerSec;
  shouldStop = shouldStop || (!m_ignoreCurrentChecks.load() && m_currentSensor &&
                              m_currentSensor.value().get().read(current) && std::abs(current) > m_maxCurrent);
  shouldStop = shouldStop || (m_upperLimit.is_connected() && !m_upperLimit.read() && m_setpoint.load() > 0);
  shouldStop = shouldStop || (m_lowerLimit.is_connected() && !m_lowerLimit.read() && m_setpoint.load() < 0);
  return shouldStop;
}

bool OpenLoop::update() {
  shouldStop() ? stop() : m_actuator.setValue(m_setpoint.load());
  return true;
}
