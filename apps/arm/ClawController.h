#pragma once

#include "ActuatorController.h"

class ClawController : public ActuatorController {

public:

    explicit ClawController(t_actuatorConfig actuatorConfig, 
                       Motor &motor, Encoder &encoder,  
                       DigitalIn &limSwitchMax, AnalogIn &forceSensor);

    mbed_error_status_t setControlMode(t_actuatorControlMode controlMode);
    
    mbed_error_status_t setMotorPower_Percentage(float percentage);
    mbed_error_status_t setVelocity_DegreesPerSec(float degreesPerSec);
    mbed_error_status_t setAngle_Degrees(float degrees);

    mbed_error_status_t setGapVelocity_CmPerSec(float cmPerSec);
    mbed_error_status_t setGapDistance_Cm(float cm);

    t_actuatorControlMode getControlMode();

    float getMotorPower_Percentage();
    float getVelocity_DegreesPerSec();
    float getAngle_Degrees();

    float getGapVelocity_CmPerSec();
    float getGapDistance_Cm();

    float getGripForce_Newtons();

    void update();

    mbed_error_status_t runPositionCalibration();

private:

    AnalogIn &r_forceSensor;

};