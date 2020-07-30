#pragma once

#include "Encoder.h"
#include "Motor.h"
#include "PID.h"
#include "PinNames.h"
#include "mbed.h"

static DigitalIn NULL_DIGITAL_IN = DigitalIn(NC);

class ActuatorController {
 public:
  typedef enum t_actuatorControlMode : uint8_t { motorPower, velocity, position } t_actuatorControlMode;

  typedef struct {
    t_actuatorControlMode defaultControlMode = motorPower;

    float minMotorPower_Percentage = -1.0, maxMotorPower_Percentage = +1.0;
    float minVelocity_DegreesPerSec = -10.0, maxVelocity_DegreesPerSec = +10.0;
    float minAngle_Degrees = -90, maxAngle_Degrees = +90;

    PID::t_pidConfig velocityPID, positionPID;

    float watchDogTimeout_Seconds = 3.0;
  } t_actuatorConfig;

  explicit ActuatorController(t_actuatorConfig actuatorConfig, Motor &motor, Encoder &encoder,
                              DigitalIn &limSwitchMin = NULL_DIGITAL_IN, DigitalIn &limSwitchMax = NULL_DIGITAL_IN);

  mbed_error_status_t setControlMode(t_actuatorControlMode controlMode);

  mbed_error_status_t setMotorPower_Percentage(float percentage);
  mbed_error_status_t setVelocity_DegreesPerSec(float degreesPerSec);
  mbed_error_status_t setAngle_Degrees(float degrees);

  mbed_error_status_t setMotionData(float motionData);

  t_actuatorControlMode getControlMode();

  float getMotorPower_Percentage();
  float getVelocity_DegreesPerSec();
  float getAngle_Degrees();

  mbed_error_status_t update();

  mbed_error_status_t resetEncoder();

  bool isLimSwitchMinTriggered();
  bool isLimSwitchMaxTriggered();
  bool isPastMinAngle();
  bool isPastMaxAngle();

  void updatePIDP(const float& value, const bool& velocityPID);
  void updatePIDI(const float& value, const bool& velocityPID);
  void updatePIDD(const float& value, const bool& velocityPID);
  void updatePIDBias(const float& value, const bool& velocityPID);
  void updatePIDDeadzone(const float& value, const bool& velocityPID);

 private:
  t_actuatorControlMode m_controlMode;
  t_actuatorConfig m_actuatorConfig;

  Motor &r_motor;
  Encoder &r_encoder;
  DigitalIn &r_limSwitchMin;
  DigitalIn &r_limSwitchMax;

  bool m_limSwitchMin_Connected;
  bool m_limSwitchMax_Connected;

  PID m_velocityPIDController;
  PID m_positionPIDController;

  Timer m_updateTimer;

  void initializePIDControllers();

  void updateActuatorConfigP(const float& value, const bool& velocityPID);
  void updateActuatorConfigI(const float& value, const bool& velocityPID);
  void updateActuatorConfigD(const float& value, const bool& velocityPID);
  void updateActuatorConfigBias(const float& value, const bool& velocityPID);
  void updateActuatorConfigDeadzone(const float& value, const bool& velocityPID);

  void updatePIDControllerP(const float& value, const bool& velocityPID);
  void updatePIDControllerI(const float& value, const bool& velocityPID);
  void updatePIDControllerD(const float& value, const bool& velocityPID);
  void updatePIDControllerBias(const float& value, const bool& velocityPID);
  void updatePIDControllerDeadzone(const float& value, const bool& velocityPID);
  // The test driver must access private data
  friend class testPIDTuningApi;
};
