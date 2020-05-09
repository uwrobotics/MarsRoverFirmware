#pragma once

#include "ActuatorController.h"
#include "Motor.h"
#include "PinNames.h"
#include "PwmIn.h"
#include "mbed.h"

// CLASS

class DifferentialWristController {
 public:
  explicit DifferentialWristController(ActuatorController &wristActuatorLeft, ActuatorController &wristActuatorRight,
                                       DigitalIn &wristLimUp, DigitalIn &WristLimCenter, DigitalIn &wristLimDown,
                                       float leftToRightMotorPowerBias = 0.0, float maxPitchAngle_Degrees = +30.0,
                                       float minPitchAngle_Degrees = -100.0, float calibrationTimeout_Seconds = 10.0);

  mbed_error_status_t setControlMode(ActuatorController::t_actuatorControlMode controlMode);

  mbed_error_status_t setRollPower_Percentage(float percentage);
  mbed_error_status_t setRollVelocity_DegreesPerSec(float degreesPerSec);
  mbed_error_status_t setRollAngle_Degrees(float degrees);

  mbed_error_status_t setPitchPower_Percentage(float percentage);
  mbed_error_status_t setPitchVelocity_DegreesPerSec(float degreesPerSec);
  mbed_error_status_t setPitchAngle_Degrees(float degrees);

  mbed_error_status_t setPitchMotionData(float motionData);
  mbed_error_status_t setRollMotionData(float motionData);

  ActuatorController::t_actuatorControlMode getControlMode();

  float getRollPower_Percentage();
  float getRollVelocity_DegreesPerSec();
  float getRollAngle_Degrees();

  float getPitchPower_Percentage();
  float getPitchVelocity_DegreesPerSec();
  float getPitchAngle_Degrees();

  void update();

  mbed_error_status_t runPositionCalibration();

 private:
  ActuatorController &r_wristActuatorLeft;
  ActuatorController &r_wristActuatorRight;

  DigitalIn &r_limSwitchUp;
  DigitalIn &r_limSwitchCenter;
  DigitalIn &r_limSwitchDown;

  bool m_limSwitchUp_Connected;
  bool m_limSwitchCenter_Connected;
  bool m_limSwitchDown_Connected;

  float m_leftToRightMotorPowerBias;

  float m_maxPitchAngle_Degrees;
  float m_minPitchAngle_Degrees;

  float m_calibrationTimeout_Seconds;

  float m_rollPower_Percentage, m_pitchPower_Percentage;
  float m_rollVelocity_DegreesPerSec, m_pitchVelocity_DegreesPerSec;
  float m_rollAngle_Degrees, m_pitchAngle_Degrees;

  mbed_error_status_t setSplitMotorPower(void);
  mbed_error_status_t setSplitVelocities(void);
  mbed_error_status_t setSplitAngles(void);

  bool isLimSwitchUpTriggered();
  bool isLimSwitchCenterTriggered();
  bool isLimSwitchDownTriggered();

  ActuatorController::t_actuatorControlMode m_controlMode;

  Mutex m_mutex;
};
