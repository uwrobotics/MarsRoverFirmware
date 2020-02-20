#include "DifferentialWristController.h"

DifferentialWristController::DifferentialWristController(ActuatorController &wristActuatorLeft, ActuatorController &wristActuatorRight, 
                                                         DigitalIn &wristLimUp, DigitalIn &wristLimCenter, DigitalIn &wristLimDown,
                                                         float leftToRightMotorPowerBias) :
        r_wristActuatorLeft(wristActuatorLeft), r_wristActuatorRight(wristActuatorRight), 
        r_wristLimUp(wristLimUp), r_wristLimCenter(wristLimCenter), r_wristLimDown(wristLimDown),
        m_leftToRightMotorPowerBias(leftToRightMotorPowerBias) {

    m_rollPower_Percentage = 0.0f;
    m_pitchPower_Percentage = 0.0f;

    m_rollVelocity_DegreesPerSec = 0.0f;
    m_pitchVelocity_DegreesPerSec = 0.0f;

    m_rollAngle_Degrees = 0.0f;

}

mbed_error_status_t DifferentialWristController::setControlMode(ActuatorController::t_actuatorControlMode controlMode) {
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorLeft.setControlMode(controlMode));
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorRight.setControlMode(controlMode));

    m_controlMode = controlMode;

    m_rollPower_Percentage = 0.0f;
    m_pitchPower_Percentage = 0.0f;

    m_rollVelocity_DegreesPerSec = 0.0f;
    m_pitchVelocity_DegreesPerSec = 0.0f;

    m_rollAngle_Degrees = 0.0f;
    m_pitchAngle_Degrees = 0.0f;

    update();

    return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setRollPower_Percentage(float percentage) {
    m_rollPower_Percentage = percentage;
    return setSplitMotorPower();
}

mbed_error_status_t DifferentialWristController::setRollVelocity_DegreesPerSec(float degreesPerSec) {
    m_rollVelocity_DegreesPerSec = degreesPerSec;
    return setSplitVelocities();
}

mbed_error_status_t DifferentialWristController::setRollAngle_Degrees(float degrees) {
    m_rollAngle_Degrees = degrees;
    return setSplitAngles();
}

mbed_error_status_t DifferentialWristController::setPitchPower_Percentage(float percentage) {
    m_pitchPower_Percentage = percentage;
    return setSplitMotorPower();
}

mbed_error_status_t DifferentialWristController::setPitchVelocity_DegreesPerSec(float degreesPerSec) {
    m_pitchVelocity_DegreesPerSec = degreesPerSec;
    return setSplitVelocities();
}

mbed_error_status_t DifferentialWristController::setPitchAngle_Degrees(float degrees) {
    m_pitchAngle_Degrees = degrees;
    return setSplitAngles();
}

mbed_error_status_t DifferentialWristController::setPitchMotionData(float motionData) {
    switch(getControlMode()) {
        case ActuatorController::t_actuatorControlMode::motorPower:
            return setPitchPower_Percentage(motionData);
        case ActuatorController::t_actuatorControlMode::velocity:
            return setPitchVelocity_DegreesPerSec(motionData);
        case ActuatorController::t_actuatorControlMode::position:
            return setPitchAngle_Degrees(motionData);
        default: 
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

mbed_error_status_t DifferentialWristController::setRollMotionData(float motionData) {
    switch(getControlMode()) {
        case ActuatorController::t_actuatorControlMode::motorPower:
            return setRollPower_Percentage(motionData);
        case ActuatorController::t_actuatorControlMode::velocity:
            return setRollVelocity_DegreesPerSec(motionData);
        case ActuatorController::t_actuatorControlMode::position:
            return setRollAngle_Degrees(motionData);
        default: 
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

ActuatorController::t_actuatorControlMode DifferentialWristController::getControlMode() {
    return m_controlMode;
}

float DifferentialWristController::getRollPower_Percentage() {
    return m_rollPower_Percentage;
}

float DifferentialWristController::getRollVelocity_DegreesPerSec() {
    return (r_wristActuatorRight.getVelocity_DegreesPerSec() - r_wristActuatorLeft.getVelocity_DegreesPerSec()) / 2.0; 
}

float DifferentialWristController::getRollAngle_Degrees() {
    return (r_wristActuatorRight.getAngle_Degrees() - r_wristActuatorLeft.getAngle_Degrees()) / 2.0; 
}

float DifferentialWristController::getPitchPower_Percentage() {
    return m_pitchPower_Percentage;
}

float DifferentialWristController::getPitchVelocity_DegreesPerSec() {
    return (r_wristActuatorLeft.getVelocity_DegreesPerSec() + r_wristActuatorRight.getVelocity_DegreesPerSec()) / 2.0; 
}
float DifferentialWristController::getPitchAngle_Degrees() {
    return (r_wristActuatorLeft.getAngle_Degrees() + r_wristActuatorRight.getAngle_Degrees()) / 2.0;
}

void DifferentialWristController::update() {
    r_wristActuatorLeft.update();
    r_wristActuatorRight.update();
}

mbed_error_status_t DifferentialWristController::runPositionCalibration() {

    // TODO

    return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setSplitMotorPower(void) {
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorLeft.setMotorPower_Percentage(-m_rollPower_Percentage + m_pitchPower_Percentage) * (1.0 + m_leftToRightMotorPowerBias));
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorRight.setMotorPower_Percentage(m_rollPower_Percentage + m_pitchPower_Percentage) * (1.0 - m_leftToRightMotorPowerBias));

    return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setSplitVelocities(void) {
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorLeft.setVelocity_DegreesPerSec(-m_rollVelocity_DegreesPerSec + m_pitchVelocity_DegreesPerSec));
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorRight.setVelocity_DegreesPerSec(m_rollVelocity_DegreesPerSec + m_pitchVelocity_DegreesPerSec));

    return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setSplitAngles(void) {
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorLeft.setAngle_Degrees(-m_rollAngle_Degrees + m_pitchAngle_Degrees));
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorRight.setAngle_Degrees(m_rollAngle_Degrees + m_pitchAngle_Degrees));

    return MBED_SUCCESS;
}
