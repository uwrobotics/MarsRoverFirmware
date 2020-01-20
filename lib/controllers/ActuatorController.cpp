
#include "ActuatorController.h"
#include <cmath>

ActuatorController::ActuatorController(Motor const * motor, Encoder const * encoder, 
					   				   DigitalIn const * limSwitchMin, 
					   				   DigitalIn const * limSwitchMax) :

		p_motor(motor), p_encoder(encoder), p_limSwitchMin(limSwitchMin), p_limSwitchMax(limSwitchMax) {

	m_limSwitchMin_Connected = (p_limSwitchMin != NULL && p_limSwitchMin->isConnected());
	m_limSwitchMax_Connected = (p_limSwitchMax != NULL && p_limSwitchMax->isConnected());

	initializePIDControllers();

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

	if ((percentage < 0.0 && m_limSwitchMin_Connected && p_limSwitchMin->read() == 1) ||
		(percentage > 0.0 && m_limSwitchMax_Connected && p_limSwitchMax->read() == 1)) {
		
		percentage = 0.0;
	}

	p_motor->setPower(percentage);
}

mbed_error_status_t setVelocity_DegreesPerSec(float degreesPerSec);
mbed_error_status_t setAngle_Degrees(float degrees);

void ActuatorController::update() {


}