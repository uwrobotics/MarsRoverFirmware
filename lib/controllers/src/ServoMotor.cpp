#include "ServoMotor.h"

ServoMotor::ServoMotor(PinName pwm, bool inverted, std::chrono::duration<double> min_pulsewidth,
                       std::chrono::duration<double> max_pulsewidth, double limit)
    : Motor(NC, NC), m_inverted(inverted), m_servo(pwm, limit, max_pulsewidth, min_pulsewidth) {}

ServoMotor::ServoMotor(t_motorConfig motorConfig) : ServoMotor(motorConfig.pwmPin, motorConfig.inverted) {}

void ServoMotor::setPower(double percentage) {
  // Convert the normalized value to the servo's actual speed
  servoSetSpeed(percentage * (m_inverted ? -1.0 : +1.0) * servoGetMaxSpeed());
}

ServoMotor& ServoMotor::operator=(double percentage) {
  this->setPower(percentage);
  return *this;
}

double ServoMotor::getPower() {
  return servoRead();
}

void ServoMotor::servoSetMaxSpeed(double max_speed_) {
  m_servo.setMaxSpeed(max_speed_);
}

void ServoMotor::servoSetSpeed(double speed_) {
  m_servo.setSpeed(speed_);
}

float ServoMotor::servoRead(void) {
  return m_servo.read();
}

float ServoMotor::servoGetMaxSpeed(void) {
  return m_servo.getMaxSpeed();
}

void ServoMotor::servoSetPeriod(std::chrono::duration<double> period) {
  m_servo.setPeriod(period);
}
