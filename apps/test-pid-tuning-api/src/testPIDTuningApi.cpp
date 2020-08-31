#include "testPIDTuningApi.h"

#include <iomanip>
#include <sstream>

#include "LookupTable.h"
#include "hw_bridge.h"

testPIDTuningApi::testPIDTuningApi()
    : testConfig{.velocityPID = {.P = defaultPID, .I = defaultPID, .D = defaultPID},
                 .positionPID = {.P = defaultPID, .I = defaultPID, .D = defaultPID}},
      testMotor(PA_1, PA_2),
      testEncoderConfig{.pin_PWM = PA_3, .degreesPerUnit = 360.0},
      testEncoder(testEncoderConfig),
      testTurnTableActuator(testConfig, testMotor, testEncoder),
      testShoulderActuator(testConfig, testMotor, testEncoder),
      testElbowActuator(testConfig, testMotor, testEncoder),
      testWristLeftActuator(testConfig, testMotor, testEncoder),
      testWristRightActuator(testConfig, testMotor, testEncoder),
      testClawController(testConfig, testMotor, testEncoder) {}

mbed_error_status_t testPIDTuningApi::setPIDParameter(CANMsg &msg) {
  printf("Received request to update PID params over CAN\n");
  /** ------------------------------------
   * | 32 bits  | 8 bits   |  8 bits     |
   * | data     | vel/pos  |  Actuator ID|
   *  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
  HWBRIDGE::ARM::PID::tuningApiPayload payload;
  msg.getPayload(payload);
  printf("payload: %s", HWBRIDGE::ARM::PID::str(payload).c_str());
  LookupTable<HWBRIDGE::ARM::ACTUATORID, std::reference_wrapper<ActuatorController>> actuatorIDLUT = {
      {HWBRIDGE::ARM::ACTUATORID::TURNTABLE, testTurnTableActuator},
      {HWBRIDGE::ARM::ACTUATORID::SHOULDER, testShoulderActuator},
      {HWBRIDGE::ARM::ACTUATORID::ELBOW, testElbowActuator},
      {HWBRIDGE::ARM::ACTUATORID::WRISTLEFT, testWristLeftActuator},
      {HWBRIDGE::ARM::ACTUATORID::WRISTRIGHT, testWristRightActuator},
      {HWBRIDGE::ARM::ACTUATORID::CLAW, testClawController}};
  auto potentialTargetActuator = actuatorIDLUT[payload.actuatorID];
  if (!potentialTargetActuator.has_value()) {
    printf("ERROR: Invalid Actuator ID\n");
    return MBED_ERROR_INVALID_ARGUMENT;
  }
  ActuatorController &targetActuator = potentialTargetActuator.value();
  switch (msg.getID()) {
    case HWBRIDGE::CANID::SET_JOINT_PID_P:
      if (targetActuator.updatePIDP(payload.value, payload.isVelocityPID)) return MBED_SUCCESS;
      break;
    case HWBRIDGE::CANID::SET_JOINT_PID_I:
      if (targetActuator.updatePIDI(payload.value, payload.isVelocityPID)) return MBED_SUCCESS;
      break;
    case HWBRIDGE::CANID::SET_JOINT_PID_D:
      if (targetActuator.updatePIDD(payload.value, payload.isVelocityPID)) return MBED_SUCCESS;
      break;
    case HWBRIDGE::CANID::SET_JOINT_PID_BIAS:
      if (targetActuator.updatePIDBias(payload.value, payload.isVelocityPID)) return MBED_SUCCESS;
      break;
    case HWBRIDGE::CANID::SET_PID_DEADZONE:
      if (targetActuator.updatePIDDeadzone(payload.value, payload.isVelocityPID)) return MBED_SUCCESS;
      break;
    default:
      break;
  }
  printf("Unable to update PID params over CAN. Ensure arm is in a safe state and update Tuning Mode.\r\n");
  printf("To allow PID param tuning over CAN, send true to CAN address SET_PID_TUNING_MODE\r\n");
  return MBED_ERROR_ASSERTION_FAILED;
}

mbed_error_status_t testPIDTuningApi::setPIDTuningMode(CANMsg &msg) {
  bool allowPIDTuning;
  bool updateFailed = false;
  msg.getPayload(allowPIDTuning);
  if (allowPIDTuning) {
    updateFailed |= testTurnTableActuator.allowPIDTuning();
    updateFailed |= testShoulderActuator.allowPIDTuning();
    updateFailed |= testElbowActuator.allowPIDTuning();
    updateFailed |= testWristLeftActuator.allowPIDTuning();
    updateFailed |= testWristRightActuator.allowPIDTuning();
    updateFailed |= testClawController.allowPIDTuning();
  } else {
    updateFailed |= testTurnTableActuator.disallowPIDTuning();
    updateFailed |= testShoulderActuator.disallowPIDTuning();
    updateFailed |= testElbowActuator.disallowPIDTuning();
    updateFailed |= testWristLeftActuator.disallowPIDTuning();
    updateFailed |= testWristRightActuator.disallowPIDTuning();
    updateFailed |= testClawController.disallowPIDTuning();
  }
  return updateFailed ? MBED_ERROR_MUTEX_LOCK_FAILED : MBED_SUCCESS;
}

void testPIDTuningApi::printAllActuatorDetails() const {
  printf("#####################################################################\n");
  printf("---------------------- TESTTURNTABLEACTUATOR ----------------------\n");
  printf("%s", getActuatorDetails(testTurnTableActuator).c_str());
  printf("---------------------- TESTSHOULDERACTUATOR -----------------------\n");
  printf("%s", getActuatorDetails(testShoulderActuator).c_str());
  printf("------------------------ TESTELBOWACTUATOR ------------------------\n");
  printf("%s", getActuatorDetails(testElbowActuator).c_str());
  printf("---------------------- TESTWRISTLEFTACTUATOR ----------------------\n");
  printf("%s", getActuatorDetails(testWristLeftActuator).c_str());
  printf("---------------------- TESTWRISTRIGHTACTUATOR ---------------------\n");
  printf("%s", getActuatorDetails(testWristRightActuator).c_str());
  printf("------------------------ TESTCLAWCONTROLLER -----------------------\n");
  printf("%s", getActuatorDetails(testClawController).c_str());
  printf("#####################################################################\n");
}

std::string testPIDTuningApi::getActuatorDetails(const ActuatorController &actuator) const {
  std::string actuatorDetails;
  actuatorDetails = "\tisVelocityPID = " + getPIDControllerDetails(actuator.m_velocityPIDController);
  actuatorDetails += "\tPosition = " + getPIDControllerDetails(actuator.m_positionPIDController);
  return actuatorDetails;
}
std::string testPIDTuningApi::getPIDControllerDetails(const PID &pid) const {
  std::string P, I, D, Deadzone, Bias;
  std::stringstream strm;
  strm << std::setprecision(4) << pid.getPParam();
  P = strm.str();
  strm.str(std::string());
  strm << std::setprecision(4) << pid.getIParam();
  I = strm.str();
  strm.str(std::string());
  strm << std::setprecision(4) << pid.getDParam();
  D = strm.str();
  strm.str(std::string());
  strm << std::setprecision(4) << pid.getDeadzoneParam();
  Deadzone = strm.str();
  strm.str(std::string());
  strm << std::setprecision(4) << pid.getBiasParam();
  Bias = strm.str();
  return "{P: " + P + ", I: " + I + ", D: " + D + ", DeadZone: " + Deadzone + ", Bias: " + Bias + "}\n";
}