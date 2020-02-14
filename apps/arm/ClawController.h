#pragma once

#include "ActuatorController.h"

class ClawController : public ActuatorController {

public:

    explicit ClawController(t_actuatorConfig actuatorConfig, 
                       Motor &motor, Encoder &encoder,  
                       DigitalIn &limSwitchMax, AnalogIn &forceSensor);

    mbed_error_status_t setGapVelocity_CmPerSec(float cmPerSec);
    mbed_error_status_t setGapDistance_Cm(float cm);

    mbed_error_status_t setMotionData(float motionData);

    float getGapVelocity_CmPerSec();
    float getGapDistance_Cm();

    float getGripForce_Newtons();

    mbed_error_status_t runPositionCalibration();

private:

    AnalogIn &r_forceSensor;

};