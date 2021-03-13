#include "LimServo.h"

using namespace Actuator;

LimServo::LimServo(PinName pin, float range, std::chrono::duration<float> max_pulse,
                   std::chrono::duration<float> min_pulse, std::chrono::duration<float> period)
    : m_pwm(pin), m_abs_range(std::abs(range)), m_pos(0) {
  // Set initial condition of PWM
  m_pwm.period(period.count());
  m_pwm = 0.0;

  // Set max and min pulse widths
  m_max_pulse = max_pulse;
  m_min_pulse = min_pulse;
}

LimServo::LimServo(const Config &config)
    : LimServo(config.pwmPin, config.range, config.max_pulse, config.min_pulse, config.period) {}

float LimServo::getRange(void) const {
  return m_abs_range;
}

void LimServo::setValue(float position) {
  int sign = (position >= 0) ? 1 : -1;
  m_pos    = (std::abs(position) < m_abs_range) ? position : m_abs_range * sign;
  // position = -1 * m_abs_range -> pwm m_min_pulse_ms; position = +1 * m_abs_range -> pwm m_max_pulse_ms
  // now, if we add m_abs_range to position, position = 0 -> pwm m_min_pulse_ms; position = 2 * m_abs_range -> pwm
  // m_max_pulse_ms now we have position as a value from 0 to 2 * m_abs_range, which can be linearly mapped to
  // m_min_pulse_ms to m_max_pulse_ms, with ease
  position += m_abs_range;
  m_pwm.pulsewidth(((m_max_pulse - m_min_pulse) * position / (m_abs_range * 2) + m_min_pulse).count());
}
