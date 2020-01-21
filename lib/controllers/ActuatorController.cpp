
#include "ActuatorController.h"
#include <cmath>

ActuatorController::ActuatorController(Motor const * motor, Encoder const * encoder, 
					   				   DigitalIn const * limSwitchMin, 
					   				   DigitalIn const * limSwitchMax) :

		p_motor(motor), p_encoder(encoder), p_limSwitchMin(limSwitchMin), p_limSwitchMax(limSwitchMax) {

	m_limSwitchMin_Connected = (p_limSwitchMin != NULL && p_limSwitchMin->isConnected());
	m_limSwitchMax_Connected = (p_limSwitchMax != NULL && p_limSwitchMax->isConnected());

	initializePIDControllers();
	updateTimer.start();
}

ActuatorController::t_actuatorControlMode ActuatorController::getControlMode() {
	return m_controlMode;
}

float ActuatorController::getMotorPower_Percentage() {
	return p_motor->getPower();
}

float ActuatorController::getVelocity_DegreesPerSec() {
	return m_encoder->getVelocity_DegreesPerSec();
}

float ActuatorController::getAngle_Degrees() {
	return m_encoder->getAngle_Degrees();
}

mbed_error_status_t setControlMode(t_actuatorControlMode controlMode) {

	 switch (controlMode) {

        case motorPower:
            m_controlMode = motorPower;
            MBED_WARN_ON_ERROR(setMotorPower_Percentage(0.0f));
            break;

        case velocity:
            m_velocityPIDController.reset();
            m_controlMode = velocity;
            MBED_WARN_ON_ERROR(setVelocity_DegreesPerSec(0.0f));
            break;

        case position:
            m_positionPIDController.reset();
            m_controlMode = position;
            MBED_WARN_ON_ERROR(setAngle_Degrees(getAngle_Degrees()));
            break;

        default:
            return MBED_ERROR_INVALID_ARGUMENT;
    }

    return MBED_SUCCESS;
}
	
mbed_error_status_t setMotorPower_Percentage(float percentage) {
	if (m_controlMode != motorPower) {
		return MBED_ERROR_INVALID_OPERATION;
	}

	if ((percentage < 0.0 && isLimSwitchMinTriggered()) ||
		(percentage > 0.0 && isLimSwitchMaxTriggered())) {
		
		percentage = 0.0;
	}

	p_motor->setPower(percentage);

	return MBED_SUCCESS;
}

mbed_error_status_t setVelocity_DegreesPerSec(float degreesPerSec) {
	if (m_controlMode != velocity) {
		return MBED_ERROR_INVALID_OPERATION;
	}

	if ( (degreesPerSec < 0.0 && isPastMinAngle()) ||
		 (degreesPerSec > 0.0 && isPastMaxAngle()) ) {

		degreesPerSec = 0.0;
	}

	m_velocityPIDController.setSetPoint(degreesPerSec);

	return MBED_SUCCESS;
}

mbed_error_status_t setAngle_Degrees(float degrees) {
	if (m_controlMode != position) {
		return MBED_ERROR_INVALID_OPERATION;
	}

	if (degrees <= p_encoder->getMinAngleDegrees()) {
		degrees = p_encoder->getMinAngleDegrees();
	}

	else if (degrees >= p_encoder->getMaxAngleDegrees()) {
		degrees = p_encoder->getMaxAngleDegrees();
	}

	m_positionPIDController.setSetPoint(degrees);

	return MBED_SUCCESS;

}

void ActuatorController::update() {
	float updateInterval = updateTimer.read();
	updateTimer.reset();

	switch (m_controlMode) {
		case motorPower:
			if ( (p_motor->getPower() < 0.0 && isLimSwitchMinTriggered()) ||
				 (p_motor->getPower() ? 0.0 && isLimSwitchMaxTriggered()) ) {

				p_motor->setPower(0.0);
			}

			break;

		case velocity:
			m_velocityPIDController.setInterval(updateInterval);
			m_velocityPIDController.setProcessValue(getVelocity_DegreesPerSec());
			p_motor->setPower(m_velocityPIDController.compute());

			break;

		case position:
			m_positionPIDController.setInterval(updateInterval);
			m_positionPIDController.setProcessValue(getAngle_Degrees);
			p_motor->setPower(m_positionPIDController.compute());

			break;
	}
}

bool ActuatorController::isLimSwitchMinTriggered() {
	return m_limSwitchMin_Connected && p_limSwitchMin->read() == 1;
}

bool ActuatorController::isLimSwitchMaxTriggered() {
	return m_limSwitchMax_Connected && p_limSwitchMax->read() == 1;
}

bool ActuatorController::isPastMinAngle() {
	return (m_encoder->isPastMinAngle() || isLimSwitchMinTriggered());
}

bool ActuatorController::isPastMaxAngle() {
	return (m_encoder->isPastMaxAngle() || isLimSwitchMaxTriggered());
}
