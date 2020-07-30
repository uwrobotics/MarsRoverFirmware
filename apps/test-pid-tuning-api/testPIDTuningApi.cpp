#include "testPIDTuningApi.h"
#include <iomanip>
#include <sstream>
#include "hw_bridge.h"

testPIDTuningApi::testPIDTuningApi()
    : testConfig{.velocityPID = {.P = defaultPID, .I = defaultPID, .D = defaultPID},
                 .positionPID = {.P = defaultPID, .I = defaultPID, .D = defaultPID}},
      testMotor(PA_1, PA_2),
      testEncoderConfig{.pin_PWM = PA_3, .degreesPerUnit = 360.0},
      testEncoder(testEncoderConfig),
      testTurnTableActuator(testConfig, testMotor, testEncoder),
      testShoulderActuator(testTurnTableActuator),
      testElbowActuator(testTurnTableActuator),
      testWristLeftActuator(testTurnTableActuator),
      testWristRightActuator(testTurnTableActuator),
      testClawController(testTurnTableActuator),
      allowPIDTuning(false) {}

mbed_error_status_t testPIDTuningApi::setPIDParameter(CANMsg &msg) {
  printf("Received request to update PID params over CAN\n");
  if (!allowPIDTuning) {
    printf("Unable to update PID params over CAN. Ensure arm is in a safe state and update Tuning Mode.\n");
    printf("To allow PID param tuning over CAN, send true to CAN address SET_PID_TUNING_MODE\n");
    return MBED_ERROR_ASSERTION_FAILED;
  }
  /** ------------------------------------
   * | 32 bits  | 8 bits   |  8 bits     |
   * | data     | vel/pos  |  Actuator ID|
   *  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
  HWBRIDGE::ARM::PID::tuningApiPayload payload;
  msg.getPayload(payload);
  printf("payload: %s", HWBRIDGE::ARM::PID::str(payload).c_str());
  ActuatorController *temp = nullptr;
  switch (payload.actuatorID) {
    case HWBRIDGE::ARM::ACTUATOR::TURNTABLE:
      temp = &testTurnTableActuator;
      break;
    case HWBRIDGE::ARM::ACTUATOR::SHOULDER:
      temp = &testShoulderActuator;
      break;
    case HWBRIDGE::ARM::ACTUATOR::ELBOW:
      temp = &testElbowActuator;
      break;
    case HWBRIDGE::ARM::ACTUATOR::WRISTLEFT:
      temp = &testWristLeftActuator;
      break;
    case HWBRIDGE::ARM::ACTUATOR::WRISTRIGHT:
      temp = &testWristRightActuator;
      break;
    case HWBRIDGE::ARM::ACTUATOR::CLAW:
      temp = &testClawController;
      break;
    default:
      printf("ERROR: Invalid Actuator ID\n");
      return MBED_ERROR_INVALID_ARGUMENT;
  }
  switch (msg.id) {
    case HWBRIDGE::CANID::SET_JOINT_PID_P:
      temp->updatePIDP(payload.value, payload.velocity);
      return MBED_SUCCESS;
    case HWBRIDGE::CANID::SET_JOINT_PID_I:
      temp->updatePIDI(payload.value, payload.velocity);
      return MBED_SUCCESS;
    case HWBRIDGE::CANID::SET_JOINT_PID_D:
      temp->updatePIDD(payload.value, payload.velocity);
      return MBED_SUCCESS;
    case HWBRIDGE::CANID::SET_PID_DEADZONE:
      temp->updatePIDDeadzone(payload.value, payload.velocity);
      return MBED_SUCCESS;
    case HWBRIDGE::CANID::SET_JOINT_PID_BIAS:
      temp->updatePIDBias(payload.value, payload.velocity);
      return MBED_SUCCESS;
    default:
      printf("ERROR: Invalid PID parameter\n");
      return MBED_ERROR_INVALID_ARGUMENT;
  }
}

mbed_error_status_t testPIDTuningApi::setPIDTuningMode(CANMsg &msg) {
  msg.getPayload(allowPIDTuning);
  return MBED_SUCCESS;
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
  actuatorDetails = "\tVelocity = " + getPIDControllerDetails(actuator.m_velocityPIDController);
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