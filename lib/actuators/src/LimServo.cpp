#include "../include/LimServo.h"

LimServo::LimServo(PinName pin) : Servo(pin) {
  m_range = DEFAULT_RANGE;
  m_pos   = 0;

  m_max_pulse = DEFAULT_MAX;
  m_min_pulse = DEFAULT_MIN;

  // INIT PWM DEFAULT
  m_pwm.period(PERIOD.count());
}

LimServo::LimServo(PinName pin, float range) : LimServo(pin) {
  m_range = range;
}

LimServo::LimServo(PinName pin, float range, std::chrono::duration<float> max_pulse,
                   std::chrono::duration<float> min_pulse)
    : LimServo(pin, range) {
  m_max_pulse = max_pulse;
  m_min_pulse = min_pulse;
}

void LimServo::setRange(float range) {
  m_range = range;
}

float LimServo::getRange(void) {
  return m_range;
}

void LimServo::setValue(float position) {
  m_pos = (std::abs(position) < m_range) ? position : m_range * getSign(position);
  // position = -1 * m_range -> pwm m_min_pulse_ms; position = +1 * m_range -> pwm m_max_pulse_ms
  // now, if we add m_range to position, position = 0 -> pwm m_min_pulse_ms; position = 2 * m_range -> pwm
  // m_max_pulse_ms now we have position as a value from 0 to 2 * m_range, which can be linearly mapped to
  // m_min_pulse_ms to m_max_pulse_ms, with ease
  position += m_range;
  m_pwm.pulsewidth(((m_max_pulse - m_min_pulse) * position / (m_range * 2) + m_min_pulse).count());
}

Actuator& LimServo::operator=(float position) {
  this->setValue(position);
  return *this;
}

float LimServo::getValue(void) {
  return m_pos;
}
