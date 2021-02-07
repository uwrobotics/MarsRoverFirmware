#include "mutex.h"

#include "Pololu37D.h"

using namespace Encoder;

Pololu37D::Pololu37D(const Config &config)
    : m_QEI(config.ChannelA, config.ChannelB, config.Index, config.quadratureEncoding),
      m_zeroOffsetDeg(config.offsetDeg) {}

bool Pololu37D::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  return (m_QEI.getPulses() * m_degreesPerCount) - m_zeroOffsetDeg;
}

bool Pololu37D::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_QEI.getPulseVelocity_PulsesPerSec() * m_degreesPerCount;
}

bool Pololu37D::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_QEI.reset();
}
