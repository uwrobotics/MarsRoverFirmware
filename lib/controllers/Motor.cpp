#include "Motor.h"

#include <cmath>

Motor::Motor(PinName pwm, PinName dir, bool inverted, int freqInHz, float limit)
    : m_pwm(pwm), m_dir(dir), m_inverted(inverted), m_limit(limit) {
  // Set initial condition of PWM
  m_pwm.period(1.0 / freqInHz);
  m_pwm = 0.0;

  // Initial condition of output enables
  m_dir = 0;

  // Set max limit to 1.0
  m_limit = fmin(m_limit, 1.0);
}

Motor::Motor(t_motorConfig motorConfig)
    : Motor(motorConfig.pwmPin, motorConfig.dirPin, motorConfig.inverted, motorConfig.freqInHz, motorConfig.limit) {}

void Motor::setPower(float dutyCycle) {
  m_dir = ((dutyCycle > 0.0) != m_inverted);
  m_pwm = fmin(fabs(dutyCycle), m_limit);
}

// TODO: this function results in a warning "control reaches end of non-void function", not sure how to fix it
Motor& Motor::operator=(int dutyCycle) {
  this->setPower(dutyCycle);
  return *this;
}

float Motor::getPower() {
  float temp;
  m_dir ? temp = m_pwm.read() : temp = -m_pwm.read();
  return temp;
}