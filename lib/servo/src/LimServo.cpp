#include "LimServo.h"

LimServo::LimServo(PinName pin) : Servo(pin) {
  m_range = DEFAULT_RANGE;
  m_pos   = 0;

  m_max_pulse = DEFAULT_MAX;
  m_min_pulse = DEFAULT_MIN;

  // INIT PWM DEFAULT
  m_pwm.period(PERIOD.count());
}

LimServo::LimServo(PinName pin, double range) : LimServo(pin) {
  m_range = range;
}

LimServo::LimServo(PinName pin, double range, std::chrono::duration<double> max_pulse,
                   std::chrono::duration<double> min_pulse)
    : LimServo(pin, range) {
  m_max_pulse = max_pulse;
  m_min_pulse = min_pulse;
}

mbed_error_status_t LimServo::setRange(double range) {
  m_range = range;
  return MBED_SUCCESS;
}

double LimServo::getRange(void) {
  return m_range;
}

mbed_error_status_t LimServo::setPosition(double angle) {
  m_pos = (std::abs(angle) < m_range) ? angle : m_range * getSign(angle);
  // angle = -1 * m_range -> pwm m_min_pulse_ms; angle = +1 * m_range -> pwm m_max_pulse_ms
  // now, if we add m_range to angle, angle = 0 -> pwm m_min_pulse_ms; angle = 2 * m_range -> pwm m_max_pulse_ms
  // now we have angle as a value from 0 to 2 * m_range, which can be linearly mapped to m_min_pulse_ms to
  // m_max_pulse_ms, with ease
  angle += m_range;
  m_pwm.pulsewidth(((m_max_pulse - m_min_pulse) * angle / (m_range * 2) + m_min_pulse).count());
  return MBED_SUCCESS;
}

double LimServo::read(void) {
  return m_pos;
}
