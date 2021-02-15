#include "MAE3.h"

#include <mutex>

using namespace Encoder;

MAE3::MAE3(const Config &config) : m_pwmIn(config.pwm), m_zeroOffsetDeg(config.offsetDeg) {}

bool MAE3::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  angle = (m_pwmIn.avgDutyCycle() * m_degreesPerUnit) - m_zeroOffsetDeg;
  return true;
}

bool MAE3::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  speed = m_pwmIn.avgDutyCycleVelocity() * m_degreesPerUnit;
  return true;
}

bool MAE3::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_zeroOffsetDeg = m_pwmIn.avgDutyCycle() * m_degreesPerUnit;
  return true;
}
