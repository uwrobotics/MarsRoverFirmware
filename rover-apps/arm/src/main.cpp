#include <cmath>

#include "CANConfig.h"
#include "ClawConfig.h"
#include "ElbowConfig.h"
#include "LimServo.h"
#include "Logger.h"
#include "ShoulderConfig.h"
#include "TooltipConfig.h"
#include "TurntableConfig.h"
#include "WristConfig.h"

CANInterface can(CANConfig::config);

// Convert degrees to radians
static inline float DEG_TO_RAD(float deg) {
  return deg * M_PI / 180.0f;
}

// Convert radians to degrees
static inline float RAD_TO_DEG(float rad) {
  return rad * 180.0f / M_PI;
}

int main() {
  Utility::logger << "";  // Band-aid fix for logger bug (issue #328)

  HWBRIDGE::CANSignalValue_t turntableSetPoint  = 0;
  HWBRIDGE::CANSignalValue_t shoulderSetPoint   = 0;
  HWBRIDGE::CANSignalValue_t elbowSetPoint      = 0;
  HWBRIDGE::CANSignalValue_t leftWristSetPoint  = 0;
  HWBRIDGE::CANSignalValue_t rightWristSetPoint = 0;
  HWBRIDGE::CANSignalValue_t clawSetPoint       = 0;
  HWBRIDGE::CANSignalValue_t toolTipPosition    = 0;

  printf("\r\n\r\n");
  printf("ARM APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  // Set CAN filters
  can.setFilter(HWBRIDGE::CANFILTER::ARM_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 0);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 1);

  while (true) {
    // *** PROCESS CAN RX SIGNALS ***

    // Determine new turntable set point
    switch (Turntable::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_POSITION,
                             turntableSetPoint);
        turntableSetPoint = RAD_TO_DEG(turntableSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_ANGULAR_VELOCITY, turntableSetPoint);
        turntableSetPoint = RAD_TO_DEG(turntableSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_CURRENT,
                             turntableSetPoint);
        break;

      default:
        break;
    }

    // Determine new shoulder set point
    switch (Shoulder::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_POSITION,
                             shoulderSetPoint);
        shoulderSetPoint = RAD_TO_DEG(shoulderSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_ANGULAR_VELOCITY, shoulderSetPoint);
        shoulderSetPoint = RAD_TO_DEG(shoulderSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_CURRENT,
                             shoulderSetPoint);
        break;

      default:
        break;
    }

    // Determine new elbow set point
    switch (Elbow::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_POSITION,
                             elbowSetPoint);
        elbowSetPoint = RAD_TO_DEG(elbowSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_ANGULAR_VELOCITY, elbowSetPoint);
        elbowSetPoint = RAD_TO_DEG(elbowSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_CURRENT,
                             elbowSetPoint);
        break;

      default:
        break;
    }

    // Determine new left wrist set point
    switch (Wrist::leftManager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_POSITION,
                             leftWristSetPoint);
        leftWristSetPoint = RAD_TO_DEG(leftWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_ANGULAR_VELOCITY, leftWristSetPoint);
        leftWristSetPoint = RAD_TO_DEG(leftWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_CURRENT,
                             leftWristSetPoint);
        break;

      default:
        break;
    }

    // Determine new right wrist set point
    switch (Wrist::rightManager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_POSITION,
                             rightWristSetPoint);
        rightWristSetPoint = RAD_TO_DEG(rightWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_ANGULAR_VELOCITY, rightWristSetPoint);
        rightWristSetPoint = RAD_TO_DEG(rightWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_CURRENT,
                             rightWristSetPoint);
        break;

      default:
        break;
    }

    // Determine new claw set point
    switch (Claw::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_POSITION,
                             clawSetPoint);
        clawSetPoint = RAD_TO_DEG(clawSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_ANGULAR_VELOCITY, clawSetPoint);
        clawSetPoint = RAD_TO_DEG(clawSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_CURRENT,
                             clawSetPoint);
        break;

      default:
        break;
    }

    // Determine new tool tip position
    can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_TOOL_TIP_POSITION,
                         toolTipPosition);
    toolTipPosition = RAD_TO_DEG(toolTipPosition);

    // *** UPDATE JOINT SET POINTS ***
    Turntable::manager.getActiveController()->setSetPoint(static_cast<float>(turntableSetPoint));
    Shoulder::manager.getActiveController()->setSetPoint(static_cast<float>(shoulderSetPoint));
    Elbow::manager.getActiveController()->setSetPoint(static_cast<float>(elbowSetPoint));
    Wrist::leftManager.getActiveController()->setSetPoint(static_cast<float>(leftWristSetPoint));
    Wrist::rightManager.getActiveController()->setSetPoint(static_cast<float>(rightWristSetPoint));
    Claw::manager.getActiveController()->setSetPoint(static_cast<float>(clawSetPoint));
    Tooltip::clawTooltipServo.setValue(static_cast<float>(toolTipPosition));

    // *** COMPUTE ACTUATOR CONTROLS ***
    Turntable::manager.getActiveController()->update();
    Elbow::manager.getActiveController()->update();
    Shoulder::manager.getActiveController()->update();
    Wrist::leftManager.getActiveController()->update();
    Wrist::rightManager.getActiveController()->update();
    Claw::manager.getActiveController()->update();

    // *** UPDATE TX SIGNALS ***

    // Joint position
    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_TURNTABLE_POSITION,
                         DEG_TO_RAD(Turntable::manager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_SHOULDER_POSITION,
                         DEG_TO_RAD(Shoulder::manager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_ELBOW_POSITION,
                         DEG_TO_RAD(Elbow::manager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_LEFT_WRIST_POSITION,
                         DEG_TO_RAD(Wrist::leftManager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_RIGHT_WRIST_POSITION,
                         DEG_TO_RAD(Wrist::rightManager.getActiveController()->reportAngleDeg()));

    // Joint angular velocity
    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_CLAW_POSITION,
                         DEG_TO_RAD(Claw::manager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_TURNTABLE_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Turntable::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_SHOULDER_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Shoulder::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_ELBOW_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Elbow::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_LEFT_WRIST_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Wrist::leftManager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_RIGHT_WRIST_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Wrist::rightManager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_CLAW_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Claw::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    // TODO: REPORT CURRENT READINGS

    // TODO: REPORT FAULTS
    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::ARM_NUM_CANRX_FAULTS,
                         can.getNumCANRXFaults());
    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::ARM_NUM_CANTX_FAULTS,
                         can.getNumCANTXFaults());

    // Report diagnostics
    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_DIAGNOSTICS,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_NUM_STREAMED_MSGS_RECEIVED, can.getNumStreamedMsgsReceived());
    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_DIAGNOSTICS,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED, can.getNumOneShotMsgsReceived());

    ThisThread::sleep_for(1ms);
  }
}

// *** HANDLERS FOR CAN RX ONE-SHOTS ***

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
    sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_ACK_ARM_SET_CONTROL_MODE_ACK);
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
    const Utility::LookupTable<HWBRIDGE::ARM_JOINT_PIDID_VALUES, Controller::ActuatorControllerManager*> lut = {
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_TURNTABLE, &Turntable::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_SHOULDER, &Shoulder::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_ELBOW, &Elbow::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_LEFT_WRIST, &Wrist::leftManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_RIGHT_WRIST, &Wrist::rightManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_CLAW, &Claw::manager}};

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
      sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_ACK_ARM_SET_JOINT_PID_PARAMS_ACK);

    } else {
      // PID controller doesn't exist!
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
    sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_ACK_CAN_BUS_SWITCH_ACK);
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
