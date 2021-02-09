#include "OpenLoop.h"

using namespace Controller;

OpenLoop::OpenLoop(Actuator::Actuator *actuator, Encoder::Encoder *encoder,
                   std::optional<Sensor::CurrentSensor *> currentSensor, float maxDegPerSec, float maxCurrent,
                   PinName lowerLimit, PinName upperLimit)
    : m_actuator(actuator),
      m_encoder(encoder),
      m_currentSensor(currentSensor),
      m_maxDegPerSec(maxDegPerSec),
      m_maxCurrent(maxCurrent),
      m_lowerLimit(lowerLimit),
      m_upperLimit(upperLimit) {}

void OpenLoop::stop() {
  setSetPoint(0);
  m_actuator->setValue(0);
}

void OpenLoop::reset() {
  stop();
  m_encoder->reset();
  if (m_currentSensor) {
    m_currentSensor.value()->reset();
  }
}

bool OpenLoop::reportAngleDeg(float &angle) {
  return m_encoder->getAngleDeg(angle);
}

bool OpenLoop::reportAngularVelocityDegPerSec(float &speed) {
  return m_encoder->getAngularVelocityDegPerSec(speed);
}

std::optional<PID::PID *> OpenLoop::getPID() {
  return std::nullopt;
}

bool OpenLoop::shouldUpdate() {
  if (!m_ignoreRPMChecks.load()) {
    float speed = 0;
    if (!m_encoder->getAngularVelocityDegPerSec(speed)) {
      return false;
    }
    if (std::abs(speed) > m_maxDegPerSec) {
      stop();
    }
  }

  if (!m_ignoreCurrentChecks.load()) {
    float current = 0;
    if (m_currentSensor) {
      if (!m_currentSensor.value()->read(current)) {
        return false;
      }
      if (std::abs(current) > m_maxCurrent) {
        stop();
      }
    }
  }

  if (m_upperLimit) {
    if (m_upperLimit.read() && m_sp.load() > 0) {
      return false;
    }
  }

  if (m_lowerLimit) {
    if (m_lowerLimit.read() && m_sp.load() < 0) {
      return false;
    }
  }
  return true;
}

bool OpenLoop::update() {
  if (shouldUpdate()) {
    m_actuator->setValue(m_sp.load());
  }
  return true;
}
