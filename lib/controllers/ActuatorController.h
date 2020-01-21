#pragma once

#include "mbed.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"

class ActuatorController {

public:

	typedef struct {
		
	} t_actuatorControllerConfig;

	typedef enum t_actuatorControlMode {
		motorPower,
		velocity,
		position
	}

	ActuatorController(Motor const * motor, Encoder const * encoder, 
					   DigitalIn const * limSwitchMin = NULL, 
					   DigitalIn const * limSwitchMax = NULL);

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

	Motor const * p_motor;
	Encoder const * p_encoder;
	DigitalIn const * p_limSwitchMin;
	DigitalIn const * p_limSwitchMax;

	bool m_limSwitchMin_Connected;
	bool m_limSwitchMax_Connected;

	PID m_velocityPIDController;
	PID m_positionPIDController;

	Timer updateTimer;

	void initializePIDControllers();

	bool isLimSwitchMinTriggered();
	bool isLimSwitchMaxTriggered();
	bool isPastMinAngle();
	bool isPastMaxAngle();
}