#include "ActuatorController.h"
#include "CANMsg.h"
#include "EncoderAbsolute_PWM.h"

class testPIDTuningApi {
 private:
  const float defaultPID = 0.0;
  ActuatorController::t_actuatorConfig testConfig;
  Motor testMotor;
  Encoder::t_encoderConfig testEncoderConfig;
  EncoderAbsolute_PWM testEncoder;
  ActuatorController testTurnTableActuator;
  ActuatorController testShoulderActuator;
  ActuatorController testElbowActuator;
  ActuatorController testWristLeftActuator;
  ActuatorController testWristRightActuator;
  ActuatorController testClawController;
  bool allowPIDTuning;
  std::string getActuatorDetails(const ActuatorController& actuator) const;
  std::string getPIDControllerDetails(const PID& pid) const;

 public:
  testPIDTuningApi();
  mbed_error_status_t setPIDParameter(CANMsg& msg);
  mbed_error_status_t setPIDTuningMode(CANMsg& msg);
  void printAllActuatorDetails() const;
};