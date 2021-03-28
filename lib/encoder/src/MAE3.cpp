#include "MAE3.h"

#include <mutex>

using namespace Encoder;

MAE3::MAE3(const Config &config) : m_pwmIn(config.pwm), m_zeroOffsetDeg(config.offsetDeg) {}

bool MAE3::update() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_angle = (m_pwmIn.avgDutyCycle() * m_degreesPerUnit) - m_zeroOffsetDeg;
  m_speed = m_pwmIn.avgDutyCycleVelocity() * m_degreesPerUnit;
  return true;
}

float MAE3::getAngleDeg() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_angle;
}

float MAE3::getAngularVelocityDegPerSec() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_speed;
}

bool MAE3::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_zeroOffsetDeg = m_pwmIn.avgDutyCycle() * m_degreesPerUnit;
  return true;
}
