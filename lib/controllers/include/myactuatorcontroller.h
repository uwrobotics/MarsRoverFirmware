#pragma once

#include "Encoder.h"
#include "DCMotor.h"
#include "PID.h"
#include "PinNames.h"
#include "mbed.h"

static DigitalIn NULL_DIGITAL_IN = DigitalIn(NC);

class myactuatorcontroller {
 public:
  enum class innerLoopMode { motorPower, velocity, position};
  typedef struct t_actuatorConfig {
    innerLoopMode defaultInnerLoopMode = innerloopmode::velocity;

    float minMotorPower_Percentage = -1.0, maxMotorPower_Percentage = +1.0;
    float minVelocity_DegreesPerSec = -10.0, maxVelocity_DegreesPerSec = +10.0;
    float minAngle_Degrees = -90, maxAngle_Degrees = +90;

    // TODO: USE THIS
    std::chrono::duration<float> watchDogTimeout = 3.0s;
  } t_actuatorConfig;

  explicit ActuatorController(t_actuatorConfig actuatorConfig, DCMotor &motor, Encoder &encoder, PID &outerPID, std::optional<PID&> innerPID = std::nullopt,
                              DigitalIn &limSwitchMin = NULL_DIGITAL_IN, DigitalIn &limSwitchMax = NULL_DIGITAL_IN);

  mbed_error_status_t setControlMode(innerLoopMode controlMode);

  mbed_error_status_t setMotorPower_Percentage(float percentage);
  mbed_error_status_t setVelocity_DegreesPerSec(float degreesPerSec);
  mbed_error_status_t setAngle_Degrees(float degrees);

  mbed_error_status_t setMotionData(float motionData);

  innerLoopMode getControlMode();

  float getMotorPower_Percentage();
  float getVelocity_DegreesPerSec();
  float getAngle_Degrees();

  mbed_error_status_t update();

  mbed_error_status_t resetEncoder();

  bool isLimSwitchMinTriggered();
  bool isLimSwitchMaxTriggered();
  bool isPastMinAngle();
  bool isPastMaxAngle();

 private:
  innerLoopMode m_controlMode;
  t_actuatorConfig m_actuatorConfig;

  DCMotor &r_motor;
  Encoder &r_encoder;
  DigitalIn &r_limSwitchMin;
  DigitalIn &r_limSwitchMax;

  bool m_limSwitchMin_Connected;
  bool m_limSwitchMax_Connected;

  PID m_velocityPIDController;
  PID m_positionPIDController;

  Timer m_updateTimer;

  void initializePIDControllers();
};
