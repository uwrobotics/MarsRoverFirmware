#include "BasicControl.h"

using namespace Controller;

BasicControl::BasicControl(Actuator::Actuator *actuator, Encoder::Encoder *encoder,
                           std::optional<Sensor::CurrentSensor *> currentSensor, PID::PID *pid, float maxDegPerSec,
                           float maxCurrent, PinName lowerLimit, PinName upperLimit)
    : m_actuator(actuator),
      m_encoder(encoder),
      m_currentSensor(currentSensor),
      m_pid(pid),
      m_maxDegPerSec(maxDegPerSec),
      m_maxCurrent(maxCurrent),
      m_lowerLimit(lowerLimit),
      m_upperLimit(upperLimit) {}

void BasicControl::stop() {
  setSetPoint(0);
  m_actuator->setValue(0);
}

void BasicControl::reset() {
  stop();
  m_encoder->reset();
  if (m_currentSensor) {
    m_currentSensor.value()->reset();
  }
  m_pid->reset();
}

std::optional<PID::PID *> BasicControl::getPID() {
  return m_pid;
}

bool BasicControl::reportAngleDeg(float &angle) {
  return m_encoder->getAngleDeg(angle);
}

bool BasicControl::reportAngularVelocityDegPerSec(float &speed) {
  return m_encoder->getAngularVelocityDegPerSec(speed);
}

bool BasicControl::shouldUpdate() {
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

  if (m_upperLimit.is_connected()) {
    if (m_upperLimit.read() && m_setpoint.load() > 0) {
      return false;
    }
  }

  if (m_lowerLimit.is_connected()) {
    if (m_lowerLimit.read() && m_setpoint.load() < 0) {
      return false;
    }
  }
  return true;
}
