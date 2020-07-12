//#include "ServoMotor.h" //TODO(qinyang-bao): fix all compile errors and renable servo code
//
//#include <cmath>
//
// ServoMotor::ServoMotor(PinName pwm, bool inverted, double min_pulsewidth_ms, double max_pulsewidth_ms, double limit)
//    : Motor(NC, NC),
//      m_servo(pwm, Servo::CONT_SERVO, limit, max_pulsewidth_ms, min_pulsewidth_ms),
//      m_inverted(inverted){};
//
// ServoMotor::ServoMotor(t_motorConfig motorConfig) : ServoMotor(motorConfig.pwmPin, motorConfig.inverted) {}
//
// void ServoMotor::setPower(double percentage) {
//  m_servo.set_speed(percentage * (m_inverted ? -1.0 : +1.0));
//}
//
// ServoMotor& ServoMotor::operator=(int percentage) {
//  this->setPower(percentage);
//  return *this;
//}
//
// double ServoMotor::getPower() {
//  return m_servo.read();
//}
