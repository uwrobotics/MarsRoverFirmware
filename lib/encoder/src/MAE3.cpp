#include "MAE3.h"

using namespace Encoder;

MAE3::MAE3(const Config &config) : m_pwmIn(config.pwm), m_zeroOffsetDeg(config.offsetDeg) {}

float MAE3::getAngleDeg() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return (m_pwmIn.avgDutyCycle() * m_degreesPerUnit) - m_zeroOffsetDeg;
}

float MAE3::getAngularVelocityDegPerSec() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_pwmIn.avgDutyCycleVelocity() * m_degreesPerUnit;
}

void MAE3::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_zeroOffsetDeg = m_pwmIn.avgDutyCycle() * m_degreesPerUnit;
}
