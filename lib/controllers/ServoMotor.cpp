#include "ServoMotor.h"

#include <cmath>

ServoMotor::ServoMotor(PinName pwm, bool inverted, float min_pulsewidth_ms, float max_pulsewidth_ms, float limit)
    : Motor(NC, NC),
      m_servo(pwm, Servo::CONT_SERVO, limit, max_pulsewidth_ms, min_pulsewidth_ms),
      m_inverted(inverted){};

ServoMotor::ServoMotor(t_motorConfig motorConfig) : ServoMotor(motorConfig.pwmPin, motorConfig.inverted) {}

void ServoMotor::setPower(float percentage) {
  m_servo.set_speed(percentage * (m_inverted ? -1.0 : +1.0));
}

ServoMotor& ServoMotor::operator=(int percentage) {
  this->setPower(percentage);
  return *this;
}

float ServoMotor::getPower() {
  return m_servo.read();
}
