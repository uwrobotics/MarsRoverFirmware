#include "Pololu37D.h"

#include <mutex>

using namespace Encoder;

Pololu37D::Pololu37D(const Config &config)
    : m_QEI(config.ChannelA, config.ChannelB, config.Index, config.quadratureEncoding),
      m_zeroOffsetDeg(config.offsetDeg) {}

bool Pololu37D::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  angle = m_current_angle_deg;
  return true;
}

bool Pololu37D::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  speed = m_speed_deg_per_second;
  return true;
}

bool Pololu37D::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_QEI.reset();
  return true;
}

bool Pololu37D::read() {
  m_previous_angle_deg = m_current_angle_deg;

  m_current_angle_deg = (m_QEI.getPulses() * m_degreesPerCount * 2) -
                        m_zeroOffsetDeg;  // multiplying by 2 since our measurements were off by a factor of 2

  m_speed_deg_per_second =
      (m_current_angle_deg - m_previous_angle_deg) * 2;  // multiply because we are measureing twice a second

  return true;
}