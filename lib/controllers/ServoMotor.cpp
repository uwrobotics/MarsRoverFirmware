#include "ServoMotor.h"
#include <cmath>

ServoMotor::ServoMotor(PinName pwm, PinName dir, bool inverted, int freqInHz, float limit) : 
        Motor(NC, NC), m_servo(pwm, Servo::CONT_SERVO), m_inverted(inverted), m_limit(limit) {};

ServoMotor::ServoMotor(t_motorConfig motorConfig) : ServoMotor(motorConfig.pwmPin, motorConfig.dirPin, motorConfig.inverted,
        motorConfig.freqInHz, motorConfig.limit) {}

void ServoMotor::setPower(float dutyCycle) {
    m_servo.set_speed(dutyCycle * (m_inverted ? -1.0 : +1.0));
}

ServoMotor& ServoMotor::operator=(int dutyCycle) {
    this->setPower(dutyCycle);
    return *this;
}

float ServoMotor::getPower() {
    return m_servo.read();
}