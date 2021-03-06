#include "Pololu37D.h"

#include <mutex>

using namespace Encoder;

Pololu37D::Pololu37D(const Config &config)
    : m_QEI(config.ChannelA, config.ChannelB, config.Index, GPIO::QEI::Encoding::X4_ENCODING),
      m_zeroOffsetDeg(config.offsetDeg) {}

bool Pololu37D::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  if (read()) {
    // reading was succesful
    angle = m_current_angle_deg;
  } else {
    return false;
  }
  return true;
}

bool Pololu37D::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  if (read()) {
    // reading was succesful
    speed = m_anglular_velocity_deg_per_sec;
  } else {
    speed = m_anglular_velocity_deg_per_sec;
    return false;
  }
  return true;
}

bool Pololu37D::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_zeroOffsetDeg                 = 0.0;
  m_current_angle_deg             = 0.0;
  m_previous_angle_deg            = 0.0;
  m_anglular_velocity_deg_per_sec = 0.0;
  m_QEI.reset();
  return true;
}

bool Pololu37D::read() {
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
    return false;
  }

  return true;
}
