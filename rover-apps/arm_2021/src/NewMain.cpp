#include <cmath>

#include "CANConfig.h"
#include "LimServo.h"
#include "Logger.h"
#include "TurntableModule.h"

CANInterface can(CANConfig::config);

// initialize modules
ClawModue claw();
ElbowModule elbow();
ShoulderModule shoulder();
TooltipModule tooltip();
TurntableModule turntable();
WristModule wrist();

// initalize polling threads
Thread periodic_1ms_thread(osPriorityRealtime);

void periodic_1ms() {
  auto startTime = Kernel::Clock::now();
  // call each module's polling function periodic_1ms

  for (Module *module : gModules) {
    module->periodic_1ms();
  }
  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::ARM_NUM_CANRX_FAULTS,
                       can.getNumCANRXFaults());
  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::ARM_NUM_CANTX_FAULTS,
                       can.getNumCANTXFaults());

  // Report diagnostics
  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_DIAGNOSTICS,
                       HWBRIDGE::CANSIGNAL::ARM_REPORT_NUM_STREAMED_MSGS_RECEIVED, can.getNumStreamedMsgsReceived());
  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_DIAGNOSTICS,
                       HWBRIDGE::CANSIGNAL::ARM_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED, can.getNumOneShotMsgsReceived());

  auto nextStartTime = startTime + 1ms;
  if (Kernel::Clock::now() > nextStartTime) {
    Utility::logger << "Periodic 1ms task failed to hit the deadline!\n";
  }
  ThisThread::sleep_until(nextStartTime);
}

int main() {
  Utility::logger << "";  // Band-aid fix for logger bug (issue #328)

  printf("\r\n\r\n");
  printf("ARM APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  // Set CAN filters
  can.setFilter(HWBRIDGE::CANFILTER::ARM_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 0);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 1);

  // Start polling threads
  periodic_1ms_thread.start(periodic_1ms);
}

// Convert degrees to radians
static inline float DEG_TO_RAD(float deg) {
  return deg * M_PI / 180.0f;
}

// Convert radians to degrees
static inline float RAD_TO_DEG(float rad) {
  return rad * 180.0f / M_PI;
}

static mbed_error_status_t armSetControlMode(void) {
  bool success = true;
  HWBRIDGE::CANSignalValue_t controlMode;

  // Set turntable control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_CONTROL_MODE, HWBRIDGE::CANSIGNAL::ARM_TURNTABLE_CONTROL_MODE,
                           controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Turntable::manager.getActiveControlMode()) {
      success &= Turntable::manager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  // Set shoulder control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_CONTROL_MODE, HWBRIDGE::CANSIGNAL::ARM_SHOULDER_CONTROL_MODE,
                           controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Shoulder::manager.getActiveControlMode()) {
      success &= Shoulder::manager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  // Set elbow control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_CONTROL_MODE, HWBRIDGE::CANSIGNAL::ARM_ELBOW_CONTROL_MODE,
                           controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Elbow::manager.getActiveControlMode()) {
      success &= Elbow::manager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  // Set left wrist control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_CONTROL_MODE, HWBRIDGE::CANSIGNAL::ARM_LEFT_WRIST_CONTROL_MODE,
                           controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Wrist::leftManager.getActiveControlMode()) {
      success &= Wrist::leftManager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  // Set right wrist control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_CONTROL_MODE, HWBRIDGE::CANSIGNAL::ARM_RIGHT_WRIST_CONTROL_MODE,
                           controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Wrist::rightManager.getActiveControlMode()) {
      success &= Wrist::rightManager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  // Set claw control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_CONTROL_MODE, HWBRIDGE::CANSIGNAL::ARM_CLAW_CONTROL_MODE,
                           controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Claw::manager.getActiveControlMode()) {
      success &= Claw::manager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  if (success) {
    // Send ACK message back
    sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_SET_CONTROL_MODE_ACK);
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t armSetJointPIDParams(void) {
  bool success = true;

  HWBRIDGE::CANSignalValue_t jointID;
  HWBRIDGE::CANSignalValue_t p, i, d, deadzone;

  // Extract signal values
  success &=
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_PID_PARAMS, HWBRIDGE::CANSIGNAL::ARM_JOINT_PIDID, jointID);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::ARM_JOINT_PID_PROPORTIONAL_GAIN, p);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::ARM_JOINT_PID_INTEGRAL_GAIN, i);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::ARM_JOINT_PID_DERIVATIVE_GAIN, d);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::ARM_JOINT_PID_DEADZONE, deadzone);

  if (success) {
    const Utility::LookupTable<HWBRIDGE::ARM_JOINT_PIDID_VALUES, Controller::ActuatorControllerManager *> lut = {
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::TURNTABLE, &Turntable::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::SHOULDER, &Shoulder::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ELBOW, &Elbow::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::LEFT_WRIST, &Wrist::leftManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::RIGHT_WRIST, &Wrist::rightManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::CLAW, &Claw::manager}};

    auto act = lut.at(static_cast<HWBRIDGE::ARM_JOINT_PIDID_VALUES>(jointID)).value_or(nullptr);
    if (!act) {
      return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Set PID params
    if (auto pid = act->getActiveController()->getPID()) {
      pid.value().get().updateProportionalGain(p);
      pid.value().get().updateIntegralGain(i);
      pid.value().get().updateDerivativeGain(d);
      pid.value().get().updateDeadzone(deadzone);

      // Send ACK message back
      sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_SET_JOINT_PID_PARAMS_ACK);

    } else {
      // PID controller doesn't exist!
      success = false;
    }
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t armSetSafetyCheck(void) {
  bool success = true;
  HWBRIDGE::CANSignalValue_t jointID;
  HWBRIDGE::CANSignalValue_t safetyCheck;

  // Get joint ID to apply safety checks to
  success = can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_SAFETY_CHECK,
                                 HWBRIDGE::CANSIGNAL::ARM_SAFETY_CHECK_JOINT_ID, jointID);

  if (success) {
    const Utility::LookupTable<HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES, Controller::ActuatorControllerManager *>
        lut = {{HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES::TURNTABLE, &Turntable::manager},
               {HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES::SHOULDER, &Shoulder::manager},
               {HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES::ELBOW, &Elbow::manager},
               {HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES::LEFT_WRIST, &Wrist::leftManager},
               {HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES::RIGHT_WRIST, &Wrist::rightManager},
               {HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES::CLAW, &Claw::manager}};

    auto act = lut.at(static_cast<HWBRIDGE::ARM_SAFETY_CHECK_JOINT_ID_VALUES>(jointID)).value_or(nullptr);
    if (!act) {
      return MBED_ERROR_INVALID_ARGUMENT;
    }
    auto temp = act->getActiveController();

    // Current check
    if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_SAFETY_CHECK, HWBRIDGE::CANSIGNAL::ARM_JOINT_CURRENT_CHECK,
                             safetyCheck)) {
      static_cast<bool>(safetyCheck) ? temp->activateCurrentChecks() : temp->deactivateCurrentChecks();
    } else {
      success = false;
    }

    // Angular velocity check
    if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_SAFETY_CHECK,
                             HWBRIDGE::CANSIGNAL::ARM_JOINT_ANGULAR_VELOCITY_CHECK, safetyCheck)) {
      static_cast<bool>(safetyCheck) ? temp->activateDegPerSecChecks() : temp->deactivateDegPerSecChecks();
    } else {
      success = false;
    }

    // Limit switch check
    if (can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_SAFETY_CHECK,
                             HWBRIDGE::CANSIGNAL::ARM_JOINT_LIMIT_SWITCH_CHECK, safetyCheck)) {
      static_cast<bool>(safetyCheck) ? temp->activateLimitSwitchChecks() : temp->deactivateLimitSwitchChecks();
    } else {
      success = false;
    }
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t commonSwitchCANBus(void) {
  bool success = true;
  HWBRIDGE::CANSignalValue_t canBusID;

  success &=
      can.getRXSignalValue(HWBRIDGE::CANID::COMMON_SWITCH_CAN_BUS, HWBRIDGE::CANSIGNAL::COMMON_CAN_BUS_ID, canBusID) &&
      can.switchCANBus(static_cast<HWBRIDGE::CANBUSID>(canBusID));

  if (success) {
    // Send ACK message back
    sendACK(HWBRIDGE::ARM_ACK_VALUES::CAN_BUS_SWITCH_ACK);
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static void sendACK(HWBRIDGE::ARM_ACK_VALUES ackValue) {
  struct uwrt_mars_rover_can_arm_report_ack_t ackMsgStruct = {
      .arm_ack = static_cast<uint8_t>(ackValue),
  };

  HWBRIDGE::CANMsgData_t ackMsgData;
  uwrt_mars_rover_can_arm_report_ack_pack(ackMsgData.raw, &ackMsgStruct, UWRT_MARS_ROVER_CAN_ARM_REPORT_ACK_LENGTH);

  CANMsg msgACK;
  msgACK.setID(HWBRIDGE::CANID::ARM_REPORT_ACK);
  msgACK.setPayload(ackMsgData, UWRT_MARS_ROVER_CAN_ARM_REPORT_ACK_LENGTH);

  can.sendOneShotMessage(msgACK, 1ms);
}
