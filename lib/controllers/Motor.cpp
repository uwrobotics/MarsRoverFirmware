#include "Motor.h"
#include "Servo.h"
#include <cmath>

Motor::Motor(PinName pwm, PinName dir, bool inverted, int freqInHz, float limit, t_motorType motorType) :
        m_pwm(pwm), m_dir(dir), m_inverted(inverted), m_limit(limit) {

    // Motor
    if(motorType == motor){
        // Set initial condition of PWM
        m_pwm.period(1.0 / freqInHz);
        m_pwm = 0.0;
    
        // Initial condition of output enables
        m_dir = 0;

        // Set max limit to 1.0
        m_limit = fmin(m_limit, 1.0);
    }
    // Limited servo
    else if(motorType == lim_servo){
        m_servo = Servo(pwm, Servo::LIM_SERVO);
    }
    // Continuous servo
    else if(motorType == cont_servo){
        m_servo = Servo(pwm, Servo::CONT_SERVO);
    }
}

Motor::Motor(t_motorConfig motorConfig, t_motorType motorType) : Motor(motorConfig.pwmPin, motorConfig.dirPin, motorConfig.inverted,
        motorConfig.freqInHz, motorConfig.limit) {
    // Motor
    if(motorType == motor)
        return;
    // Limited servo
    else if(motorType == lim_servo)
        m_servo = Servo(motorConfig.pwmPin, Servo::LIM_SERVO);
    // Continuous servo
    else if(motorType == cont_servo)
        m_servo = Servo(motorConfig.pwmPin, Servo::CONT_SERVO);
}

void Motor::setPower(float dutyCycle) {
    if(motorType == motor){
        m_dir = ((dutyCycle > 0.0) != m_inverted);
        m_pwm = fmin(fabs(dutyCycle), m_limit);
    }
}

Motor& Motor::operator=(int dutyCycle) {
    if(motorType == motor){
        this->setPower(dutyCycle);
        return *this;
    }
}

float Motor::getPower() {
    float temp;
    m_dir?temp = m_pwm.read():temp = -m_pwm.read();
    return temp;
}

Motor::t_motorType Motor::getType() {
    return motorType;
}

bool Motor::servoSetRange(float range_) {
    if(motorType == lim_servo)
    return (m_servo.setRange(range_));
} 

bool Motor::servoSetMaxSpeed(float max_speed_) {
    if(motorType == cont_servo)
    return (m_servo.setMaxSpeed(max_speed_));
}

bool Motor::servoSetPosition(float angle) {
    if(motorType == lim_servo)
    return (m_servo.setPosition(angle));
}

bool Motor::servoSetSpeed(float speed_) {
    if(motorType == cont_servo)
    return (m_servo.setSpeed(speed_));
}

float Motor::servoRead(void) {
    return m_servo.read();
}

float Motor::servoGetMaxSpeed(void) {
    return m_servo.getMaxSpeed();
}

void Motor::servoSetPeriod(int period) {
    m_servo.setPeriod(period);
}