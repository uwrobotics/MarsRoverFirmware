#include "ContServo.h"

ContServo::ContServo(PinName pin) : m_pin(pin), m_pwm(pin) {
  m_max_speed = 0;
  m_speed     = 0;

  m_max_pulse = DEFAULT_MAX;
  m_min_pulse = DEFAULT_MIN;

  // INIT PWM DEFAULT
  m_pwm.period(PERIOD.count());
}

ContServo::ContServo(PinName pin, float max_speed) : ContServo(pin) {
  m_max_speed = max_speed;
}

ContServo::ContServo(PinName pin, float max_speed, std::chrono::duration<float> max_pulse,
                     std::chrono::duration<float> min_pulse)
    : ContServo(pin, max_speed) {
  m_max_pulse = max_pulse;
  m_min_pulse = min_pulse;
}

void ContServo::setMaxSpeed(float max_speed) {
  m_max_speed = max_speed;
}

float ContServo::getMaxSpeed(void) const {
  return m_max_speed;
}

void ContServo::setValue(float speed) {
  m_speed = (std::abs(speed) < m_max_speed) ? speed : m_max_speed * getSign(speed);

  // So now speed is from 0 to 2 * m_max_speed
  speed += m_max_speed;
  m_pwm.pulsewidth(((m_max_pulse - m_min_pulse) * speed / (m_max_speed * 2) + m_min_pulse).count());
}

ContServo& ContServo::operator=(float speed) {
  setValue(speed);
  return *this;
}

float ContServo::getValue(void) const {
  return m_speed;
}
