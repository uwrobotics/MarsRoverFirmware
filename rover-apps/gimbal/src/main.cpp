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
  HWBRIDGE::CANSignalValue_t panSetPoint;
  HWBRIDGE::CANSignalValue_t pitchPosition;
  HWBRIDGE::CANSignalValue_t rollPosition;

  printf("\r\n\r\n");
  printf("GIMBAL APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  // Set CAN filters
  can.setFilter(HWBRIDGE::CANFILTER::GIMBAL_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 0);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 1);

  while (true) {
    // Process CAN RX signals (TODO: NEED TO HANDLE SNA CASES)
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

    can.readStreamedSignal(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_PITCH_POSITION,
                           pitchPosition);
    pitchPosition = RAD_TO_DEG(pitchPosition);
    can.readStreamedSignal(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_ROLL_POSITION,
                           rollPosition);
    rollPosition = RAD_TO_DEG(rollPosition);

    // Update joint set points
    Pan::manager.getActiveController()->setSetPoint((float)panSetPoint);
    Pitch::pitchServo.setValue((float)pitchPosition);

    // Compute actuator controls
    Pan::manager.getActiveController()->update();

    // Update TX signals
    can.updateStreamedSignal(
        HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA, HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_POSITION,
        (HWBRIDGE::CANSignalValue_t)DEG_TO_RAD(Pan::manager.getActiveController()->reportAngleDeg()));
    can.updateStreamedSignal(
        HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA, HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_ANGULAR_VELOCITY,
        (HWBRIDGE::CANSignalValue_t)DEG_TO_RAD(Pan::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    ThisThread::sleep_for(1ms);
  }
}

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
