#pragma once

#include "ActuatorController.h"
#include "Servo.h"
#include "FSR.h"

class ClawController : public ActuatorController {

public:

    explicit ClawController(t_actuatorConfig actuatorConfig, 
                       Motor &motor, Encoder &encoder,  
                       DigitalIn &limSwitchMax, FSR &forceSensor, Servo &tooltipServo,
                       float tooltipExtendedAngle_Degrees = 180.0, float tooltipRetractedAngle_Degrees = 0.0,
                       float calibrationTimeout_Seconds = 10.0, float maxForceFSR_Newtons = 40.0);

    mbed_error_status_t setMotorPower_Percentage(float percentage);

    mbed_error_status_t setGapVelocity_CmPerSec(float cmPerSec);
    mbed_error_status_t setGapDistance_Cm(float cm);

    mbed_error_status_t setMotionData(float motionData);

    mbed_error_status_t extendToolTip();
    mbed_error_status_t retractToolTip();

    float getGapVelocity_CmPerSec();
    float getGapDistance_Cm();

    float getGripForce_Newtons();

    mbed_error_status_t runPositionCalibration();

    mbed_error_status_t update();

private:

    FSR &r_forceSensor;
    Servo    &r_tooltipServo;

    float m_max_force_fsr_newtons;
    float m_tooltipExtendedAngle_Degrees;
    float m_tooltipRetractedAngle_Degrees;

    float m_calibrationTimeout_Seconds;

    float convertShaftPositionDegreesToGapCm(float shaftPosition_Degrees);
    float convertShaftVelocityDegreesToGapVelocityCm(float shaftPosition_DegreesPerSec);
    float convertGapCmToShaftPositionDegrees(float gap_cm);
    float convertGapVelocityCmToShaftVelocityDegrees(float gap_cmPerSec);

    Mutex m_mutex;

};