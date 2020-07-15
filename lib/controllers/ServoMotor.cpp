#include "ServoMotor.h"

#include <cmath>

ServoMotor::ServoMotor(PinName pwm, bool inverted, float min_pulsewidth_ms, float max_pulsewidth_ms, float limit)
    : Motor(NC, NC), m_inverted(inverted), m_servo(pwm, limit, max_pulsewidth_ms, min_pulsewidth_ms) {}

ServoMotor::ServoMotor(t_motorConfig motorConfig) : ServoMotor(motorConfig.pwmPin, motorConfig.inverted) {}

void ServoMotor::setPower(float percentage) {
  // Convert the normalized value to the servo's actual speed
  servoSetSpeed(percentage * (m_inverted ? -1.0 : +1.0) * servoGetMaxSpeed());
}

ServoMotor& ServoMotor::operator=(int percentage) {
  this->setPower(percentage);
  return *this;
}

float ServoMotor::getPower() {
  return servoRead();
}

void ServoMotor::servoSetMaxSpeed(float max_speed_) {
  m_servo.setMaxSpeed(max_speed_);
}

void ServoMotor::servoSetSpeed(float speed_) {
  m_servo.setSpeed(speed_);
}

float ServoMotor::servoRead(void) {
  return m_servo.read();
}

float ServoMotor::servoGetMaxSpeed(void) {
  return m_servo.getMaxSpeed();
}

void ServoMotor::servoSetPeriod(int period) {
  m_servo.setPeriod(period);
}