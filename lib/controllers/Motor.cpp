#include "Motor.h"

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
        m_servo = new LimServo(pwm);
    }
    // Continuous servo
    else if(motorType == cont_servo){
        m_servo = new ContServo(pwm);
    }
}

Motor::Motor(t_motorConfig motorConfig, t_motorType motorType) : Motor(motorConfig.pwmPin, motorConfig.dirPin, motorConfig.inverted,
        motorConfig.freqInHz, motorConfig.limit) {
    // Motor
    if(motorType == motor)
        return;
    // Limited servo
    else if(motorType == lim_servo)
        m_servo = new LimServo(motorConfig.pwmPin);
    // Continuous servo
    else if(motorType == cont_servo)
        m_servo = new ContServo(motorConfig.pwmPin);
}

Motor::~Motor(){
    delete m_servo;
}

void Motor::setPower(float dutyCycle) {
    if(motorType == motor){
        m_dir = ((dutyCycle > 0.0) != m_inverted);
        m_pwm = fmin(fabs(dutyCycle), m_limit);
    }
}

// TODO: this function results in a warning "control reaches end of non-void function", not sure how to fix it
Motor& Motor::operator=(int dutyCycle) {
    if(motorType == motor){
        this->setPower(dutyCycle);
    }
    
    return *this;
}

float Motor::getPower() {
    float temp;
    m_dir?temp = m_pwm.read():temp = -m_pwm.read();
    return temp;
}

Motor::t_motorType Motor::getType() {
    return motorType;
}

// These methods, if not called on the correct servo type, will return false or -1
bool Motor::servoSetRange(float range_) {
    return m_servo->setRange(range_);
} 

bool Motor::servoSetMaxSpeed(float max_speed_) {
    return m_servo->setMaxSpeed(max_speed_);
}

bool Motor::servoSetPosition(float angle) {
    return m_servo->setPosition(angle);
}

bool Motor::servoSetSpeed(float speed_) {
    return m_servo->setSpeed(speed_);
}

float Motor::servoRead(void) {
    return m_servo->read();
}

float Motor::servoGetMaxSpeed(void) {
    return m_servo->getMaxSpeed();
}

void Motor::servoSetPeriod(int period) {
    m_servo->setPeriod(period);
}