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

// TODO: Add CAN handler for enabling/disabling current checks.
// TODO: Add CAN handler for enabling/disabling rpm checks
// TODO: Add CAN handler for enabling/disabling current checks

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
  can.setFilter(HWBRIDGE::CANFILTER::ARM_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  while (true) {
    // *** PROCESS CAN RX SIGNALS (TODO: NEED TO HANDLE SNA CASES) ***

    // Determine new turntable set point
    switch (Turntable::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_POSITION,
                               turntableSetPoint);
        turntableSetPoint = RAD_TO_DEG(turntableSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_ANGULAR_VELOCITY, turntableSetPoint);
        turntableSetPoint = RAD_TO_DEG(turntableSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_CURRENT,
                               turntableSetPoint);
        break;

      default:
        break;
    }

    // Determine new shoulder set point
    switch (Shoulder::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_POSITION,
                               shoulderSetPoint);
        shoulderSetPoint = RAD_TO_DEG(shoulderSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_ANGULAR_VELOCITY, shoulderSetPoint);
        shoulderSetPoint = RAD_TO_DEG(shoulderSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_CURRENT,
                               shoulderSetPoint);
        break;

      default:
        break;
    }

    // Determine new elbow set point
    switch (Elbow::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_POSITION,
                               elbowSetPoint);
        elbowSetPoint = RAD_TO_DEG(elbowSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_ANGULAR_VELOCITY, elbowSetPoint);
        elbowSetPoint = RAD_TO_DEG(elbowSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_CURRENT,
                               elbowSetPoint);
        break;

      default:
        break;
    }

    // Determine new left wrist set point
    switch (Wrist::leftManager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION,
                               HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_POSITION, leftWristSetPoint);
        leftWristSetPoint = RAD_TO_DEG(leftWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_ANGULAR_VELOCITY, leftWristSetPoint);
        leftWristSetPoint = RAD_TO_DEG(leftWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_CURRENT,
                               leftWristSetPoint);
        break;

      default:
        break;
    }

    // Determine new right wrist set point
    switch (Wrist::rightManager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION,
                               HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_POSITION, rightWristSetPoint);
        rightWristSetPoint = RAD_TO_DEG(rightWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_ANGULAR_VELOCITY, rightWristSetPoint);
        rightWristSetPoint = RAD_TO_DEG(rightWristSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_CURRENT,
                               rightWristSetPoint);
        break;

      default:
        break;
    }

    // Determine new claw set point
    switch (Claw::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_POSITION,
                               clawSetPoint);
        clawSetPoint = RAD_TO_DEG(clawSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_ANGULAR_VELOCITY, clawSetPoint);
        clawSetPoint = RAD_TO_DEG(clawSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_CURRENT,
                               clawSetPoint);
        break;

      default:
        break;
    }

    // Determine new tool tip position
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_TOOL_TIP_POSITION,
                           toolTipPosition);
    toolTipPosition = RAD_TO_DEG(toolTipPosition);

    // *** UPDATE JOINT SET POINTS ***
    Turntable::manager.getActiveController()->setSetPoint((float)turntableSetPoint);
    Shoulder::manager.getActiveController()->setSetPoint((float)shoulderSetPoint);
    Elbow::manager.getActiveController()->setSetPoint((float)elbowSetPoint);
    Wrist::leftManager.getActiveController()->setSetPoint((float)leftWristSetPoint);
    Wrist::rightManager.getActiveController()->setSetPoint((float)rightWristSetPoint);
    Claw::manager.getActiveController()->setSetPoint((float)clawSetPoint);
    Tooltip::clawTooltipServo.setValue((float)toolTipPosition);

    // *** COMPUTE ACTUATOR CONTROLS ***
    Turntable::manager.getActiveController()->update();
    Elbow::manager.getActiveController()->update();
    Shoulder::manager.getActiveController()->update();
    Wrist::leftManager.getActiveController()->update();
    Wrist::rightManager.getActiveController()->update();
    Claw::manager.getActiveController()->update();

    // *** UPDATE TX SIGNALS ***

    // Joint position
    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_TURNTABLE_POSITION,
                             DEG_TO_RAD(Turntable::manager.getActiveController()->reportAngleDeg()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_SHOULDER_POSITION,
                             DEG_TO_RAD(Shoulder::manager.getActiveController()->reportAngleDeg()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_ELBOW_POSITION,
                             DEG_TO_RAD(Elbow::manager.getActiveController()->reportAngleDeg()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_LEFT_WRIST_POSITION,
                             DEG_TO_RAD(Wrist::leftManager.getActiveController()->reportAngleDeg()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_RIGHT_WRIST_POSITION,
                             DEG_TO_RAD(Wrist::rightManager.getActiveController()->reportAngleDeg()));

    // Joint angular velocity
    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_CLAW_POSITION,
                             DEG_TO_RAD(Claw::manager.getActiveController()->reportAngleDeg()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_TURNTABLE_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Turntable::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_SHOULDER_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Shoulder::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_ELBOW_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Elbow::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_LEFT_WRIST_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Wrist::leftManager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_RIGHT_WRIST_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Wrist::rightManager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_REPORT_CLAW_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Claw::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    // TODO: REPORT CURRENT READINGS

    // TODO: REPORT FAULTS

    // Report CAN diagnostics
    uint32_t numMsgsReceived = can.getNumMsgsReceived();

    can.updateStreamedSignal(HWBRIDGE::CANID::ARM_REPORT_DIAGNOSTICS, HWBRIDGE::CANSIGNAL::ARM_REPORT_CAN_STATS,
                             numMsgsReceived);

    printf("ARM - number of CAN messages received: %lu\r\n", numMsgsReceived);

    ThisThread::sleep_for(1ms);
  }
}

// *** HANDLERS FOR CAN RX ONE-SHOTS ***

static mbed_error_status_t armSetControlMode(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::ARM_SET_CONTROL_MODE) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CONTROL::Mode controlMode;
  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_arm_set_control_mode_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_arm_set_control_mode_unpack(&msgStruct, msgData.raw,
                                                      UWRT_MARS_ROVER_CAN_ARM_SET_CONTROL_MODE_LENGTH) == 0) {
    // Set turntable control mode
    controlMode = (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_arm_set_control_mode_arm_turntable_control_mode_decode(
        msgStruct.arm_turntable_control_mode);
    success &= Turntable::manager.switchControlMode(controlMode);

    // Set shoulder control mode
    controlMode = (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_arm_set_control_mode_arm_shoulder_control_mode_decode(
        msgStruct.arm_shoulder_control_mode);
    success &= Shoulder::manager.switchControlMode(controlMode);

    // Set elbow control mode
    controlMode = (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_arm_set_control_mode_arm_elbow_control_mode_decode(
        msgStruct.arm_elbow_control_mode);
    success &= Elbow::manager.switchControlMode(controlMode);

    // Set left wrist control mode
    controlMode = (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_arm_set_control_mode_arm_left_wrist_control_mode_decode(
        msgStruct.arm_left_wrist_control_mode);
    success &= Wrist::leftManager.switchControlMode(controlMode);

    // Set right wrist control mode
    controlMode = (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_arm_set_control_mode_arm_right_wrist_control_mode_decode(
        msgStruct.arm_right_wrist_control_mode);
    success &= Wrist::rightManager.switchControlMode(controlMode);

    // Set claw control mode
    controlMode = (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_arm_set_control_mode_arm_claw_control_mode_decode(
        msgStruct.arm_claw_control_mode);
    success &= Claw::manager.switchControlMode(controlMode);

    if (success) {
      // Send ACK message back
      sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_ACK_ARM_SET_CONTROL_MODE_ACK);
    }
  } else {
    // Error unpacking!
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t armSetJointPIDParams(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::ARM_SET_JOINT_PID_PARAMS) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_arm_set_joint_pid_params_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_arm_set_joint_pid_params_unpack(&msgStruct, msgData.raw,
                                                          UWRT_MARS_ROVER_CAN_ARM_SET_JOINT_PID_PARAMS_LENGTH) == 0) {
    // Determine joint ID
    HWBRIDGE::ARM_JOINT_PIDID_VALUES jointID =
        (HWBRIDGE::ARM_JOINT_PIDID_VALUES)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pidid_decode(
            msgStruct.arm_joint_pidid);

    const Utility::LookupTable<HWBRIDGE::ARM_JOINT_PIDID_VALUES, Controller::ActuatorControllerManager*> lut = {
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_TURNTABLE, &Turntable::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_SHOULDER, &Shoulder::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_ELBOW, &Elbow::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_LEFT_WRIST, &Wrist::leftManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_RIGHT_WRIST, &Wrist::rightManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_CLAW, &Claw::manager}};

    auto act = lut.at(jointID).value_or(nullptr);
    if (!act) {
      return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Extract PID params
    float p = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_proportional_gain_decode(
        msgStruct.arm_joint_pid_proportional_gain);
    float i = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_integral_gain_decode(
        msgStruct.arm_joint_pid_integral_gain);
    float d = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_derivative_gain_decode(
        msgStruct.arm_joint_pid_derivative_gain);
    float deadzone = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_deadzone_decode(
        msgStruct.arm_joint_pid_proportional_gain);

    // Set PID params
    if (auto pid = act->getActiveController()->getPID()) {
      if (msgStruct.arm_joint_pid_proportional_gain !=
          (uint16_t)HWBRIDGE::ARM_JOINT_PID_PROPORTIONAL_GAIN_VALUES::ARM_JOINT_PID_PROPORTIONAL_GAIN_SNA) {
        pid.value().get().updateProportionalGain(p);
      }
      if (msgStruct.arm_joint_pid_integral_gain !=
          (uint16_t)HWBRIDGE::ARM_JOINT_PID_INTEGRAL_GAIN_VALUES::ARM_JOINT_PID_INTEGRAL_GAIN_SNA) {
        pid.value().get().updateIntegralGain(i);
      }
      if (msgStruct.arm_joint_pid_derivative_gain !=
          (uint16_t)HWBRIDGE::ARM_JOINT_PID_DERIVATIVE_GAIN_VALUES::ARM_JOINT_PID_DERIVATIVE_GAIN_SNA) {
        pid.value().get().updateDerivativeGain(d);
      }
      if (msgStruct.arm_joint_pid_deadzone !=
          (uint16_t)HWBRIDGE::ARM_JOINT_PID_DEADZONE_VALUES::ARM_JOINT_PID_DEADZONE_SNA) {
        pid.value().get().updateDeadzone(deadzone);
      }

      // Send ACK message back
      sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_ACK_ARM_SET_JOINT_PID_PARAMS_ACK);

    } else {
      // PID controller doesn't exist!
      success = false;
    }
  } else {
    // Error unpacking!
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t commonSwitchCANBus(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::COMMON_SWITCH_CAN_BUS) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_common_switch_can_bus_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_common_switch_can_bus_unpack(&msgStruct, msgData.raw,
                                                       UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_LENGTH) == 0) {
    success &= can.switchCANBus((HWBRIDGE::CANBUSID)uwrt_mars_rover_can_common_switch_can_bus_common_can_bus_id_decode(
        msgStruct.common_can_bus_id));

    // Send ACK message back
    sendACK(HWBRIDGE::ARM_ACK_VALUES::ARM_ACK_CAN_BUS_SWITCH_ACK);
  } else {
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static void sendACK(HWBRIDGE::ARM_ACK_VALUES ackValue) {
  struct uwrt_mars_rover_can_arm_report_ack_t ackMsgStruct = {
      .arm_ack = (uint8_t)ackValue,
  };

  HWBRIDGE::CANMsgData_t ackMsgData;
  uwrt_mars_rover_can_arm_report_ack_pack(ackMsgData.raw, &ackMsgStruct, UWRT_MARS_ROVER_CAN_ARM_REPORT_ACK_LENGTH);

  CANMsg msgACK;
  msgACK.setID(HWBRIDGE::CANID::ARM_REPORT_ACK);
  msgACK.setPayload(ackMsgData, UWRT_MARS_ROVER_CAN_ARM_REPORT_ACK_LENGTH);

  can.sendOneShotMessage(msgACK, 1ms);
}
