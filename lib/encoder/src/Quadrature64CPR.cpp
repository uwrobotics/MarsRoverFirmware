#include "Quadrature64CPR.h"

#include <mutex>

using namespace Encoder;

Quadrature64CPR::Quadrature64CPR(const Config &config)
    : m_QEI(config.ChannelA, config.ChannelB, config.Index, GPIO::QEI::Encoding::X4_ENCODING),
      m_zeroOffsetDeg(config.offsetDeg) {}

float Quadrature64CPR::getAngleDeg() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_current_angle_deg;
}

float Quadrature64CPR::getAngularVelocityDegPerSec() {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_anglular_velocity_deg_per_sec;
}

bool Quadrature64CPR::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
  update();

  m_zeroOffsetDeg = m_current_angle_deg;

  m_current_angle_deg  = 0.0;
  m_previous_angle_deg = 0.0;

  m_anglular_velocity_deg_per_sec = 0.0;
  m_QEI.reset();

  return true;
}

bool Quadrature64CPR::update() {
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
    // TODO log this as an error considering dt = 0
  }

  return true;
}
