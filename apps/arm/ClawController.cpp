#include "ClawController.h"

ClawController::ClawController(t_actuatorConfig actuatorConfig, 
                       Motor &motor, Encoder &encoder,  
                       DigitalIn &limSwitchMax, AnalogIn &forceSensor) : 
        ActuatorController(actuatorConfig, motor, encoder, NULL_DIGITAL_IN, limSwitchMax), r_forceSensor(forceSensor) {};


mbed_error_status_t ClawController::setGapVelocity_CmPerSec(float cmPerSec) {
    float shaftVelocity_DegreesPerSec = 2.0 * cmPerSec + 0.0 * getGapDistance_Cm(); // TODO find mapping function (velocity dependent on position)
    return setVelocity_DegreesPerSec(shaftVelocity_DegreesPerSec);
}

mbed_error_status_t ClawController::setGapDistance_Cm(float cm) {
    float shaftPosition_Degrees = 2.0 * cm; // TODO find mapping function
    return setAngle_Degrees(shaftPosition_Degrees);
}

mbed_error_status_t ClawController::setMotionData(float motionData) {
    switch(getControlMode()) {
        case t_actuatorControlMode::motorPower:
            return setMotorPower_Percentage(motionData);
        case t_actuatorControlMode::velocity:
            return setGapVelocity_CmPerSec(motionData);
        case t_actuatorControlMode::position:
            return setGapDistance_Cm(motionData);
        default: 
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

float ClawController::getGapVelocity_CmPerSec() {
    return getVelocity_DegreesPerSec() / 2.0 + 0.0 * getAngle_Degrees(); // TODO find mapping function (velocity dependent on position)
}
float ClawController::getGapDistance_Cm() {
    return getAngle_Degrees() / 2.0; // TODO find mapping function
}

float ClawController::getGripForce_Newtons() {
    return 0.0; // TODO
}

mbed_error_status_t ClawController::runPositionCalibration() {
    return MBED_SUCCESS; // TODO
}