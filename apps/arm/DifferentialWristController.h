#pragma once

#include "mbed.h"
#include "Motor.h"
#include "PwmIn.h"
#include "PinNames.h"
#include "ActuatorController.h"

// CLASS

class DifferentialWristController {

public:

    DifferentialWristController(ActuatorController &wristActuatorLeft, ActuatorController &wristActuatorRight, 
                                DigitalIn &wristLimUp, DigitalIn &WristLimCenter, DigitalIn &wristLimDown,
                                float leftToRightMotorPowerBias = 0.0);

    mbed_error_status_t setControlMode(ActuatorController::t_actuatorControlMode controlMode);
    
    mbed_error_status_t setRollPower_Percentage(float percentage);
    mbed_error_status_t setRollVelocity_DegreesPerSec(float degreesPerSec);
    mbed_error_status_t setRollAngle_Degrees(float degrees);

    mbed_error_status_t setPitchPower_Percentage(float percentage);
    mbed_error_status_t setPitchVelocity_DegreesPerSec(float degreesPerSec);
    mbed_error_status_t setPitchAngle_Degrees(float degrees);

    ActuatorController::t_actuatorControlMode getControlMode();

    float getRollPower_Percentage();
    float getRollVelocity_DegreesPerSec();
    float getRollAngle_Degrees();    

    float getPitchPower_Percentage();
    float getPitchVelocity_DegreesPerSec();
    float getPitchAngle_Degrees();

    void update();

private:

    ActuatorController &r_wristActuatorLeft;
    ActuatorController &r_wristActuatorRight;

    DigitalIn &r_wristLimUp;
    DigitalIn &r_wristLimCenter;
    DigitalIn &r_wristLimDown;

    float m_leftToRightMotorPowerBias;

    float m_rollPower_Percentage, m_pitchPower_Percentage;
    float m_rollVelocity_DegreesPerSec, m_pitchVelocity_DegreesPerSec;
    float m_rollAngle_Degrees, m_pitchAngle_Degrees;

    mbed_error_status_t setSplitMotorPower(void);
    mbed_error_status_t setSplitVelocities(void);
    mbed_error_status_t setSplitAngles(void);

    ActuatorController::t_actuatorControlMode m_controlMode;

};