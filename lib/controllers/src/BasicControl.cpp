#include "BasicControl.h"

using namespace Controller;

BasicControl(Actuator *actuator, Encoder *encoder, CurrentSensor *currentSensor, PID *pid, float maxDegPerSec,
             float maxCurrent, std::optional<DigitalIn *> lowerLimit, std::optional<DigitalIn *> upperLimit)
    : m_actuator(actuator),
      m_encoder(encoder),
      m_currentSensor(currentSensor),
      m_pid(pid),
      m_maxDegPerSec(maxDegPerSec),
      m_maxCurrent(maxCurrent) m_lowerLimit(lowerLimit),
      m_upperLimit(upperLimit) {}

void BasicControl::stop() {
  setSetPoint(0);
  m_actuator->setValue(0);
}

void BasicControl::reset() {
  stop();
  m_encoder->reset();
  m_currentSensor->reset();
  m_pid->reset();
}

std::optional<PID::PID *> BasicControl::getPID() {
  return m_pid;
}

bool BasicControl::reportAngleDeg(float &angle) {
  return m_encoder->getAngle(angle);
}

bool BasicControl::reportAngularVelocityDegPerSec(float &speed) {
  return m_encoder->getAngularVelocityDegPerSec(speed);
}
