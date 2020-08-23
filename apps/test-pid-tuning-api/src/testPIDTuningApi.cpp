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
  return MBED_SUCCESS;
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