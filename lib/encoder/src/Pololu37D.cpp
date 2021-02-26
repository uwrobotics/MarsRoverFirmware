#include "Pololu37D.h"

#include <mutex>

using namespace Encoder;

Pololu37D::Pololu37D(const Config &config)
    : m_QEI(config.ChannelA, config.ChannelB, config.Index, config.quadratureEncoding),
      m_zeroOffsetDeg(config.offsetDeg) {}

bool Pololu37D::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  read();
  angle = m_current_angle_deg;
  return true;
}

bool Pololu37D::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  read();
  speed = m_speed_deg_per_second;
  return true;
}

bool Pololu37D::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_QEI.reset();
  return true;
}

float Pololu37D::return_delta_time() {
  return m_delta_time_ns;
}

bool Pololu37D::read() {
  // time since last measurement
  m_timer.stop();
  m_delta_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(m_timer.elapsed_time()).count();
  m_timer.reset();
  m_timer.start();

  m_previous_angle_deg = m_current_angle_deg;

  m_current_angle_deg = (m_QEI.getPulses() * m_degreesPerCount * 2) -
                        m_zeroOffsetDeg;  // multiplying by 2 since our measurements were off by a factor of 2

  // to estimate speed: delta_angle / delta_time (degrees / sec)
  m_speed_deg_per_second = (m_current_angle_deg - m_previous_angle_deg) / m_delta_time_ns * 1000000000;

  return true;
}