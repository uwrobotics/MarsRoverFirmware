#include "DifferentialWristController.h"

#include "ThisThread.h"

DifferentialWristController::DifferentialWristController(ActuatorController &wristActuatorLeft,
                                                         ActuatorController &wristActuatorRight, DigitalIn &wristLimUp,
                                                         DigitalIn &wristLimCenter, DigitalIn &wristLimDown,
                                                         float leftToRightMotorPowerBias, float maxPitchAngle_Degrees,
                                                         float minPitchAngle_Degrees, std::chrono::duration<double> calibrationTimeout)
    : r_wristActuatorLeft(wristActuatorLeft),
      r_wristActuatorRight(wristActuatorRight),
      r_limSwitchUp(wristLimUp),
      r_limSwitchCenter(wristLimCenter),
      r_limSwitchDown(wristLimDown),
      m_leftToRightMotorPowerBias(leftToRightMotorPowerBias),
      m_maxPitchAngle_Degrees(maxPitchAngle_Degrees),
      m_minPitchAngle_Degrees(minPitchAngle_Degrees),
      m_calibrationTimeout(calibrationTimeout) {
  m_limSwitchUp_Connected     = (r_limSwitchUp != NULL_DIGITAL_IN && r_limSwitchUp.is_connected());
  m_limSwitchCenter_Connected = (r_limSwitchCenter != NULL_DIGITAL_IN && r_limSwitchCenter.is_connected());
  m_limSwitchDown_Connected   = (r_limSwitchDown != NULL_DIGITAL_IN && r_limSwitchDown.is_connected());

  m_rollPower_Percentage  = 0.0f;
  m_pitchPower_Percentage = 0.0f;

  m_rollVelocity_DegreesPerSec  = 0.0f;
  m_pitchVelocity_DegreesPerSec = 0.0f;

  m_rollAngle_Degrees = 0.0f;
}

mbed_error_status_t DifferentialWristController::setControlMode(ActuatorController::t_actuatorControlMode controlMode) {
  MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorLeft.setControlMode(controlMode));
  MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorRight.setControlMode(controlMode));

  m_controlMode = controlMode;

  m_rollPower_Percentage  = 0.0f;
  m_pitchPower_Percentage = 0.0f;

  m_rollVelocity_DegreesPerSec  = 0.0f;
  m_pitchVelocity_DegreesPerSec = 0.0f;

  m_rollAngle_Degrees  = 0.0f;
  m_pitchAngle_Degrees = 0.0f;

  return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setRollPower_Percentage(float percentage) {
  mbed_error_status_t errStatus;

  if (m_mutex.trylock_for(200ms)) {
    m_rollPower_Percentage = percentage;
    errStatus              = setSplitMotorPower();
  } else {
    return MBED_ERROR_TIME_OUT;
  }

  m_mutex.unlock();

  return errStatus;
}

mbed_error_status_t DifferentialWristController::setRollVelocity_DegreesPerSec(float degreesPerSec) {
  mbed_error_status_t errStatus;

  if (m_mutex.trylock_for(200ms)) {
    m_rollVelocity_DegreesPerSec = degreesPerSec;
    errStatus                    = setSplitVelocities();
  } else {
    return MBED_ERROR_TIME_OUT;
  }

  m_mutex.unlock();

  return errStatus;
}

mbed_error_status_t DifferentialWristController::setRollAngle_Degrees(float degrees) {
  mbed_error_status_t errStatus;

  if (m_mutex.trylock_for(200ms)) {
    m_rollAngle_Degrees = degrees;
    errStatus           = setSplitAngles();
  } else {
    return MBED_ERROR_TIME_OUT;
  }

  m_mutex.unlock();

  return errStatus;
}

mbed_error_status_t DifferentialWristController::setPitchPower_Percentage(float percentage) {
  mbed_error_status_t errStatus;

  if (m_mutex.trylock_for(200ms)) {
    m_pitchPower_Percentage = percentage;
    errStatus               = setSplitMotorPower();
  } else {
    return MBED_ERROR_TIME_OUT;
  }

  m_mutex.unlock();

  return errStatus;
}

mbed_error_status_t DifferentialWristController::setPitchVelocity_DegreesPerSec(float degreesPerSec) {
  mbed_error_status_t errStatus;

  if (m_mutex.trylock_for(200ms)) {
    m_pitchVelocity_DegreesPerSec = degreesPerSec;
    errStatus                     = setSplitVelocities();
  } else {
    return MBED_ERROR_TIME_OUT;
  }

  m_mutex.unlock();

  return errStatus;
}

mbed_error_status_t DifferentialWristController::setPitchAngle_Degrees(float degrees) {
  mbed_error_status_t errStatus;

  if (m_mutex.trylock_for(200ms)) {
    m_pitchAngle_Degrees = degrees;
    errStatus            = setSplitAngles();
  } else {
    return MBED_ERROR_TIME_OUT;
  }

  m_mutex.unlock();

  return errStatus;
}

mbed_error_status_t DifferentialWristController::setPitchMotionData(float motionData) {
  switch (getControlMode()) {
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
  switch (getControlMode()) {
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
  Timer calibrationTimer;

  ActuatorController::t_actuatorControlMode prevControlMode = getControlMode();

  if (m_mutex.trylock_for(1s)) {
    calibrationTimer.start();

    setControlMode(ActuatorController::velocity);
    setRollVelocity_DegreesPerSec(5.0);

    while (!isLimSwitchCenterTriggered() && calibrationTimer.elapsed_time() < m_calibrationTimeout) {
      update();
      ThisThread::sleep_for(2ms);
    }

    setRollVelocity_DegreesPerSec(0.0);

    // Settle
    ThisThread::sleep_for(750ms);

    setPitchVelocity_DegreesPerSec(5.0);  // Pitch upwards

    while (!isLimSwitchUpTriggered() && calibrationTimer.elapsed_time() < m_calibrationTimeout) {
      update();
      ThisThread::sleep_for(2ms);
    }

    setPitchVelocity_DegreesPerSec(0.0);

    // Settle
    ThisThread::sleep_for(750ms);

    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorLeft.resetEncoder());
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorRight.resetEncoder());

    setControlMode(ActuatorController::position);

    setPitchAngle_Degrees(-m_maxPitchAngle_Degrees);

    while ((getPitchAngle_Degrees() > (-m_maxPitchAngle_Degrees + 2.0)) &&
           calibrationTimer.elapsed_time() < m_calibrationTimeout) {
      update();
      ThisThread::sleep_for(2ms);
    }

    // Settle
    ThisThread::sleep_for(750ms);

    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorLeft.resetEncoder());
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(r_wristActuatorRight.resetEncoder());

    setControlMode(prevControlMode);

  } else {
    return MBED_ERROR_MUTEX_LOCK_FAILED;
  }

  m_mutex.unlock();

  return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setSplitMotorPower(void) {
  if (m_mutex.trylock_for(200ms)) {
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(
        r_wristActuatorLeft.setMotorPower_Percentage(-m_rollPower_Percentage + m_pitchPower_Percentage) *
        (1.0 + m_leftToRightMotorPowerBias));
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(
        r_wristActuatorRight.setMotorPower_Percentage(m_rollPower_Percentage + m_pitchPower_Percentage) *
        (1.0 - m_leftToRightMotorPowerBias));
  } else {
    return MBED_ERROR_TIME_OUT;
  }

  m_mutex.unlock();

  return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setSplitVelocities(void) {
  if (m_mutex.trylock_for(200ms)) {
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(
        r_wristActuatorLeft.setVelocity_DegreesPerSec(-m_rollVelocity_DegreesPerSec + m_pitchVelocity_DegreesPerSec));
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(
        r_wristActuatorRight.setVelocity_DegreesPerSec(m_rollVelocity_DegreesPerSec + m_pitchVelocity_DegreesPerSec));
  } else {
    return MBED_ERROR_MUTEX_LOCK_FAILED;
  }

  m_mutex.unlock();

  return MBED_SUCCESS;
}

mbed_error_status_t DifferentialWristController::setSplitAngles(void) {
  if (m_mutex.trylock_for(200ms)) {
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(
        r_wristActuatorLeft.setAngle_Degrees(-m_rollAngle_Degrees + m_pitchAngle_Degrees));
    MBED_WARN_AND_RETURN_STATUS_ON_ERROR(
        r_wristActuatorRight.setAngle_Degrees(m_rollAngle_Degrees + m_pitchAngle_Degrees));
  } else {
    return MBED_ERROR_MUTEX_LOCK_FAILED;
  }

  m_mutex.unlock();

  return MBED_SUCCESS;
}

bool DifferentialWristController::isLimSwitchUpTriggered() {
  return m_limSwitchUp_Connected && r_limSwitchUp.read() == 0;  // Open drain
}

bool DifferentialWristController::isLimSwitchCenterTriggered() {
  return m_limSwitchCenter_Connected && r_limSwitchCenter.read() == 0;  // Open drain
}

bool DifferentialWristController::isLimSwitchDownTriggered() {
  return m_limSwitchDown_Connected && r_limSwitchDown.read() == 0;  // Open drain
}
