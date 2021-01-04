#include "DCMotor.h"

#include <cmath>

DCMotor::DCMotor::DCMotor(PinName pwm, PinName dir, bool inverted, int freqInHz, float limit)
    : m_pwm(pwm), m_dir(dir), m_inverted(inverted), m_limit(fmin(limit, 1.0)) {
  // Set initial condition of PWM
  m_pwm.period(1.0 / freqInHz);
  m_pwm = 0.0;

  // Initial condition of output enables
  m_dir = 0;
}

DCMotor::DCMotor::DCMotor(const Config &config)
    : DCMotor(config.pwmPin, config.dirPin, config.inverted, config.freqInHz, config.limit) {}

void DCMotor::DCMotor::setValue(float dutyCycle) {
  m_dir = ((dutyCycle > 0.0) != m_inverted);
  m_pwm = fmin(fabs(dutyCycle), m_limit);
}

float DCMotor::DCMotor::getValue() const {
  return m_dir ? m_pwm.read() : -m_pwm.read();
}
