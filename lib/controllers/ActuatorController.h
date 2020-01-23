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
	} t_actuatorControlMode;

	typedef struct {
		t_actuatorControlMode defaultControlMode;

		float minMotorPower_Percentage, maxMotorPower_Percentage;
		float minVelocity_DegreesPerSec, maxVelocity_DegreesPerSec;
		float minAngle_Degrees, maxAngle_Degrees;

		PID::t_pidConfig velocityPID, positionPID;
	} t_actuatorConfig;

	ActuatorController(t_actuatorConfig actuatorConfig, 
					   Motor * motor, Encoder * encoder, 
					   DigitalIn * limSwitchMin = NULL, 
					   DigitalIn * limSwitchMax = NULL);

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
	t_actuatorConfig m_actuatorConfig;

	Motor * p_motor;
	Encoder * p_encoder;
	DigitalIn * p_limSwitchMin;
	DigitalIn * p_limSwitchMax;

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
};