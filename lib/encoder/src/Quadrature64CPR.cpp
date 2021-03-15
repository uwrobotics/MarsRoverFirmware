#include "Quadrature64CPR.h"

#include <mutex>

using namespace Encoder;

Quadrature64CPR::Quadrature64CPR(const Config &config)
    : m_QEI(config.ChannelA, config.ChannelB, config.Index, GPIO::QEI::Encoding::X4_ENCODING),
      m_zeroOffsetDeg(config.offsetDeg) {}

bool Quadrature64CPR::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  read();
  angle = m_current_angle_deg;
  return true;
}

bool Quadrature64CPR::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  read();
  speed = m_anglular_velocity_deg_per_sec;
  return true;
}

bool Quadrature64CPR::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  read();

  m_zeroOffsetDeg = m_current_angle_deg;

  m_current_angle_deg  = 0.0;
  m_previous_angle_deg = 0.0;

  m_anglular_velocity_deg_per_sec = 0.0;
  m_QEI.reset();

  return true;
}

void Quadrature64CPR::read() {
  // time since last measurement in nanoseconds
  m_timer.stop();
  float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_timer.elapsed_time()).count();
  m_timer.reset();
  m_timer.start();

  m_previous_angle_deg = m_current_angle_deg;

  m_current_angle_deg = (m_QEI.getPulses() * m_degreesPerCount) - m_zeroOffsetDeg;

  if (dt != 0) {
    // to estimate speed: delta_angle / delta_time (degrees / sec)
    m_anglular_velocity_deg_per_sec = (m_current_angle_deg - m_previous_angle_deg) / dt * 1000000000;
  } else {
    // to avoid div by zero
    m_anglular_velocity_deg_per_sec = 0;
  }
}
