#include "ContServo.h"

ContServo::ContServo(PinName pin) : Servo(pin) {
  m_rotate_type = CONT_SERVO;

  m_max_speed = 0;
  m_speed     = 0;

  m_max_pulse_ms = DEFAULT_MAX;
  m_min_pulse_ms = DEFAULT_MIN;

  // INIT PWM DEFAULT
  m_pwm.period_us(PERIOD * 1000);
}

ContServo::ContServo(PinName pin, float max_speed) : ContServo(pin) {
  m_max_speed = max_speed;
}

ContServo::ContServo(PinName pin, float max_speed, float max_pulse_ms, float min_pulse_ms) : ContServo(pin, max_speed) {
  m_max_pulse_ms = max_pulse_ms;
  m_min_pulse_ms = min_pulse_ms;
}

bool ContServo::setMaxSpeed(float max_speed) {
  m_max_speed = max_speed;
  return true;
}

float ContServo::getMaxSpeed(void) {
  return m_max_speed;
}

bool ContServo::setSpeed(float speed) {
  m_speed = (std::abs(speed) < m_max_speed) ? speed : m_max_speed * getSign(speed);
  // So now speed is from 0 to 2 * m_max_speed
  speed += m_max_speed;
  m_pwm.pulsewidth_us(int(((m_max_pulse_ms - m_min_pulse_ms) * speed / (m_max_speed * 2) + m_min_pulse_ms) * 1000));
  return true;
}

float ContServo::read(void) {
  return m_speed;
}
