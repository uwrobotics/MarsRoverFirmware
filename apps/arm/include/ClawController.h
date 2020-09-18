#pragma once

#include "ActuatorController.h"
#include "LimServo.h"

class ClawController : public ActuatorController {
 public:
  explicit ClawController(t_actuatorConfig actuatorConfig, Motor &motor, Encoder &encoder, DigitalIn &limSwitchMax,
                          AnalogIn &forceSensor, LimServo &tooltipServo, float tooltipExtendedAngle_Degrees = 180.0,
                          float tooltipRetractedAngle_Degrees = 0.0, chrono::duration<double> calibrationTimeout = 10s);

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

 private:
  AnalogIn &r_forceSensor;
  LimServo &r_tooltipServo;

  float m_tooltipExtendedAngle_Degrees;
  float m_tooltipRetractedAngle_Degrees;

  std::chrono::duration<float> m_calibrationTimeout;

  float convertShaftPositionDegreesToGapCm(float shaftPosition_Degrees);
  float convertShaftVelocityDegreesToGapVelocityCm(float shaftPosition_DegreesPerSec);
  float convertGapCmToShaftPositionDegrees(float gap_cm);
  float convertGapVelocityCmToShaftVelocityDegrees(float gap_cmPerSec);

  Mutex m_mutex;
};
