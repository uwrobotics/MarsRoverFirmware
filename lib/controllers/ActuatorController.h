#pragma once

#include "mbed.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"

class ActuatorController {

public:

	typedef enum t_actuatorControlMode {
		motorPower,
		velocity,
		position
	}

	ActuatorController();

	mbed_error_status_t setControlMode(t_actuatorControlMode controlMode);
	mbed_error_status_t setMotorPower_Percentage(float percentage);
	mbed_error_status_t setVelocity_DegreesPerSec(float degreesPerSec);
	mbed_error_status_t setAngle_Degrees(float degrees);

	t_actuatorControlMode getControlMode();

	float getMotorPower_Percentage();
	float getVelocity_DegreesPerSec();
	float getAngle_Degrees();

	void update();

private:

	t_actuatorControlMode m_controlMode;

	Motor m_motor;
	Encoder m_encoder;
	DigitalIn m_limSwitchMin, m_limSwitchMax;

	PID m_velocityPIDController;
	PID m_positionPIDController;

}