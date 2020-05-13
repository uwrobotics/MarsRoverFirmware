#include "ServoMotor.h"

#include <cmath>

ServoMotor::ServoMotor(PinName pwm, bool inverted, float min_pulsewidth_ms, float max_pulsewidth_ms, float limit) : 
			Motor(NC, NC), m_inverted(inverted) 
{
	// Motor(NC, NC) has default motorType=motor, so m_servo is not set to anything at this point
    m_servo = new ContServo(pwm, limit, max_pulsewidth_ms, min_pulsewidth_ms);
  
}

ServoMotor::ServoMotor(t_motorConfig motorConfig) : ServoMotor(motorConfig.pwmPin, motorConfig.inverted) {}

void ServoMotor::setPower(float percentage) {
    m_servo->setSpeed(percentage * (m_inverted ? -1.0 : +1.0));
}

ServoMotor& ServoMotor::operator=(int percentage) {
  this->setPower(percentage);
  return *this;
}

float ServoMotor::getPower() {
    return m_servo->read();
}