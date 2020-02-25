#include "../targets/gimbal/pinNames.h"

#include "mbed.h"
#include "mbed_config.h"
#include "can_config.h"

#include "GimbalConfig.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "ActuatorController.h"
#include "CANMsg.h"
#include "CANBuffer.h"
#include "Servo.h"

// Init. Components

// Servos
Servo panServo(SRVO_PWM_CR, Servo::CONT_SERVO, 44.0, 2.1, 0.9); //44 RPM at 4.8V, max->2100us PW, min->900us PW
Servo tiltServo(SRVO_PWM_MG, Servo::LIM_SERVO, 180, 2.1, 0.9);

// Motors
Motor panServoMotor(GimbConfig::panMotorConfig, Motor::cont_servo);
Motor tiltServoMotor(GimbConfig::tiltMotorConfig, Motor::lim_servo);

// Encoders
EncoderAbsolute_PWM panEncoder(GimbConfig::panEncoderConfig);

// Limit switche
DigitalIn tiltLimUp(LIM_GIMB);

// Rotary actuator
ActuatorController panActuator(GimbConfig::panActuatorConfig, panServoMotor, panEncoder);
ActuatorController tiltActuator(GimbConfig::tiltActuatorConfig, tiltServoMotor, panEncoder); // panEncoder as placeholder, will not be called.

int main()
{
    while (true) {
    }
}
