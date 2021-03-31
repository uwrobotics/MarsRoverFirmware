#include "Pololu37D.h"

#include <mutex>

using namespace Encoder;

Pololu37D::Pololu37D(const Config &config)
    : m_QEI(config.ChannelA, config.ChannelB, config.Index, config.quadratureEncoding),
      m_zeroOffsetDeg(config.offsetDeg) {}

bool Pololu37D::update() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_angle = (m_QEI.getPulses() * m_degreesPerCount) - m_zeroOffsetDeg;
  m_speed = m_QEI.getPulseVelocity_PulsesPerSec() * m_degreesPerCount;
  return true;
}
float Pololu37D::getAngleDeg() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_angle;
}

float Pololu37D::getAngularVelocityDegPerSec() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_speed;
}

bool Pololu37D::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_QEI.reset();
  return true;
}
