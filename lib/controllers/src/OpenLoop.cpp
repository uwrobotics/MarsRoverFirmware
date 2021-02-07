#include "OpenLoop.h"

using namespace Controller;

OpenLoop::OpenLoop(Actuator *actuator, Encoder *encoder, float maxDegPerSec, CurrentSensor *currentSensor,
                   float maxCurrent, std::optional<DigitalIn *> lowerLimit, std::optional<DigitalIn *> upperLimit)
    : m_actuator(actuator),
      m_encoder(encoder),
      m_maxDegPerSec(maxDegPerSec),
      m_currentSensor(currentSensor),
      m_maxCurrent(maxCurrent),
      m_lowerLimit(lowerLimit),
      m_upperLimit(upperLimit) {}

void OpenLoop::stop() {
  setSetPoint(0);
  m_actuator->seValue(0);
}

void OpenLoop::reset() {
  stop();
  m_encoder->reset();
  m_currentSensor->reset();
}

bool OpenLoop::reportAngleDeg(float &angle) {
  return m_encoder->getAngle(angle);
}

bool OpenLoop::reportAngularVelocityDegPerSec(float &speed) {
  return m_encoder->getAngularVelocityDegPerSec(speed);
}


void OpenLoop::update() {
  if (!m_ignoreRPMChecks.load()) {
    if (std::abs(m_encoder->getVelocity_DegreesPerSec()) > m_maxDegPerSec) {
      stop();
    }
  }
  if (!m_ignoreCurrentChecks.load()) {
    if (std::abs(m_encoder->read()) > m_maxDegPerSec) {
      stop();
    }
  }
  if (m_upperLimit) {
    if (m_upperLimit.value()->read() && m_sp.load() > 0) {
      return;
    }
  }
  if (m_lowerLimit) {
    if (m_lowerLimit.value()->read() && m_sp.load() < 0) {
      return;
    }
  }
  m_actuator->setValue(m_sp.load());
}

std::optional<PID::PID *> OpenLoop::getPID() {
  return std::nullopt;
}