#include "AEAT6012.h"

using namespace Encoder;

AEAT6012::AEAT6012(const Config &config) : m_pin(config.pin), m_offsetDeg(config.offsetDeg) {}

float AEAT6012::getAngleDeg() {
  std::scoped_lock<Mutex> lock(m_mutex);
  MBED_ASSERT(false);
  return 0;
}

float AEAT6012::getAngularVelocityDegPerSec() {
  std::scoped_lock<Mutex> lock(m_mutex);
  MBED_ASSERT(false);
  return 0;
}

void AEAT6012::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  MBED_ASSERT(false);
}
