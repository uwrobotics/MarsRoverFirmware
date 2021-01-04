#include "ContServo.h"

ContServo::ContServo::ContServo(PinName pin, float max_speed, std::chrono::duration<float> max_pulse,
                                std::chrono::duration<float> min_pulse, std::chrono::duration<float> period)
    : m_pwm(pin), m_abs_max_speed(max_speed), m_speed(0) {
  // Set initial condition of PWM
  m_pwm.period(period.count());
  m_pwm = 0.0;

  // Set max and min pulse widths
  m_max_pulse = max_pulse;
  m_min_pulse = min_pulse;
}

ContServo::ContServo::ContServo(const Config &config)
    : ContServo(config.pwmPin, config.max_speed, config.max_pulse, config.min_pulse, config.period) {}

float ContServo::ContServo::getMaxSpeed(void) const {
  return m_abs_max_speed;
}

void ContServo::ContServo::setValue(float speed) {
  int sign = (speed >= 0) ? 1 : -1;
  m_speed  = (std::abs(speed) < m_abs_max_speed) ? speed : m_abs_max_speed * sign;

  // So now speed is from 0 to 2 * m_abs_max_speed
  speed += m_abs_max_speed;
  m_pwm.pulsewidth(((m_max_pulse - m_min_pulse) * speed / (m_abs_max_speed * 2) + m_min_pulse).count());
}

float ContServo::ContServo::getValue(void) const {
  return m_speed;
}
