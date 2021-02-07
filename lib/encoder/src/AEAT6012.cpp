#include "AEAT6012.h"

using namespace Encoder;

AEAT6012::AEAT6012(const Config &config) : m_pin(config.pin), m_offsetDeg(config.offsetDeg) {}

bool AEAT6012::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  MBED_ASSERT(false);
  return false;
}

bool AEAT6012::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  MBED_ASSERT(false);
  return false;
}

bool AEAT6012::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  MBED_ASSERT(false);
  return false;
}
