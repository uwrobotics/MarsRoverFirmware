#include "CANConfig.h"
#include "CANMsg.h"
#include "Logger.h"
#include "PanConfig.h"
#include "PinNames.h"
#include "PitchConfig.h"
#include "hw_bridge.h"

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

  HWBRIDGE::CANSignalValue_t panSetPoint;
  HWBRIDGE::CANSignalValue_t pitchPosition;

  printf("\r\n\r\n");
  printf("GIMBAL APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  // Set CAN filters
  can.setFilter(HWBRIDGE::CANFILTER::GIMBAL_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  while (true) {
    // *** PROCESS CAN RX SIGNALS (TODO: NEED TO HANDLE SNA CASES) ***

    // Determine new pan position
    switch (Pan::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_PAN_POSITION,
                               panSetPoint);
        panSetPoint = RAD_TO_DEG(panSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::GIMBAL_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::GIMBAL_SET_PAN_ANGULAR_VELOCITY, panSetPoint);
        panSetPoint = RAD_TO_DEG(panSetPoint);
        break;

      default:
        break;
    }

    // Determine new pitch position
    can.readStreamedSignal(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_PITCH_POSITION,
                           pitchPosition);
    pitchPosition = RAD_TO_DEG(pitchPosition);

    // *** UPDATE JOINT SET POINTS ***
    Pan::manager.getActiveController()->setSetPoint((float)panSetPoint);
    Pitch::pitchServo.setValue((float)pitchPosition);

    // *** COMPUTE ACTUATOR CONTROLS ***
    Pan::manager.getActiveController()->update();

    // *** UPDATE TX SIGNALS ***
    can.updateStreamedSignal(HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA, HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_POSITION,
                             DEG_TO_RAD(Pan::manager.getActiveController()->reportAngleDeg()));

    can.updateStreamedSignal(HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA,
                             HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Pan::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    // TODO: REPORT FAULTS

    // TODO: REPORT CAN DIAGNOSTICS

    ThisThread::sleep_for(1ms);
  }
}

// *** HANDLERS FOR CAN RX ONE-SHOTS ***

static mbed_error_status_t gimbalSetControlMode(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::GIMBAL_SET_CONTROL_MODE) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CONTROL::Mode controlMode;
  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_gimbal_set_control_mode_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_gimbal_set_control_mode_unpack(&msgStruct, msgData.raw,
                                                         UWRT_MARS_ROVER_CAN_GIMBAL_SET_CONTROL_MODE_LENGTH) == 0) {
    // Set pan control mode
    controlMode = (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_gimbal_set_control_mode_gimbal_pan_control_mode_decode(
        msgStruct.gimbal_pan_control_mode);
    success &= Pan::manager.switchControlMode(controlMode);

    if (success) {
      // Send ACK message back
      sendACK(HWBRIDGE::GIMBAL_ACK_VALUES::GIMBAL_ACK_GIMBAL_SET_CONTROL_MODE_ACK);
    }
  } else {
    // Error unpacking!
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t gimbalSetJointPIDParams(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::GIMBAL_SET_JOINT_PID_PARAMS) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_gimbal_set_joint_pid_params_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_gimbal_set_joint_pid_params_unpack(
          &msgStruct, msgData.raw, UWRT_MARS_ROVER_CAN_GIMBAL_SET_JOINT_PID_PARAMS_LENGTH) == 0) {
    // Error check joint ID (only allow PID tuning of pan axis)
    HWBRIDGE::GIMBAL_JOINT_PIDID_VALUES jointID =
        (HWBRIDGE::GIMBAL_JOINT_PIDID_VALUES)uwrt_mars_rover_can_gimbal_set_joint_pid_params_gimbal_joint_pidid_decode(
            msgStruct.gimbal_joint_pidid);

    if (jointID != HWBRIDGE::GIMBAL_JOINT_PIDID_VALUES::GIMBAL_JOINT_PIDID_PAN) {
      return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Extract PID params
    float p = (float)uwrt_mars_rover_can_gimbal_set_joint_pid_params_gimbal_joint_pid_proportional_gain_decode(
        msgStruct.gimbal_joint_pid_proportional_gain);
    float i = (float)uwrt_mars_rover_can_gimbal_set_joint_pid_params_gimbal_joint_pid_integral_gain_decode(
        msgStruct.gimbal_joint_pid_integral_gain);
    float d = (float)uwrt_mars_rover_can_gimbal_set_joint_pid_params_gimbal_joint_pid_derivative_gain_decode(
        msgStruct.gimbal_joint_pid_derivative_gain);
    float deadzone = (float)uwrt_mars_rover_can_gimbal_set_joint_pid_params_gimbal_joint_pid_deadzone_decode(
        msgStruct.gimbal_joint_pid_proportional_gain);

    // Set PID params
    if (auto pid = Pan::manager.getActiveController()->getPID()) {
      if (msgStruct.gimbal_joint_pid_proportional_gain !=
          (uint16_t)HWBRIDGE::GIMBAL_JOINT_PID_PROPORTIONAL_GAIN_VALUES::GIMBAL_JOINT_PID_PROPORTIONAL_GAIN_SNA) {
        pid.value().get().updateProportionalGain(p);
      }
      if (msgStruct.gimbal_joint_pid_integral_gain !=
          (uint16_t)HWBRIDGE::GIMBAL_JOINT_PID_INTEGRAL_GAIN_VALUES::GIMBAL_JOINT_PID_INTEGRAL_GAIN_SNA) {
        pid.value().get().updateIntegralGain(i);
      }
      if (msgStruct.gimbal_joint_pid_derivative_gain !=
          (uint16_t)HWBRIDGE::GIMBAL_JOINT_PID_DERIVATIVE_GAIN_VALUES::GIMBAL_JOINT_PID_DERIVATIVE_GAIN_SNA) {
        pid.value().get().updateDerivativeGain(d);
      }
      if (msgStruct.gimbal_joint_pid_deadzone !=
          (uint16_t)HWBRIDGE::GIMBAL_JOINT_PID_DEADZONE_VALUES::GIMBAL_JOINT_PID_DEADZONE_SNA) {
        pid.value().get().updateDeadzone(deadzone);
      }

      // Send ACK message back
      sendACK(HWBRIDGE::GIMBAL_ACK_VALUES::GIMBAL_ACK_GIMBAL_SET_JOINT_PID_PARAMS_ACK);

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
    sendACK(HWBRIDGE::GIMBAL_ACK_VALUES::GIMBAL_ACK_CAN_BUS_SWITCH_ACK);
  } else {
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static void sendACK(HWBRIDGE::GIMBAL_ACK_VALUES ackValue) {
  struct uwrt_mars_rover_can_gimbal_report_ack_t ackMsgStruct = {
      .gimbal_ack = (uint8_t)ackValue,
  };

  HWBRIDGE::CANMsgData_t ackMsgData;
  uwrt_mars_rover_can_gimbal_report_ack_pack(ackMsgData.raw, &ackMsgStruct,
                                             UWRT_MARS_ROVER_CAN_GIMBAL_REPORT_ACK_LENGTH);

  CANMsg msgACK;
  msgACK.setID(HWBRIDGE::CANID::GIMBAL_REPORT_ACK);
  msgACK.setPayload(ackMsgData, UWRT_MARS_ROVER_CAN_GIMBAL_REPORT_ACK_LENGTH);

  can.sendOneShotMessage(msgACK, 1ms);
}
