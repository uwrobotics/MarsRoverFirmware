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
        can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_PAN_POSITION,
                             panSetPoint);
        panSetPoint = RAD_TO_DEG(panSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::GIMBAL_SET_PAN_ANGULAR_VELOCITY, panSetPoint);
        panSetPoint = RAD_TO_DEG(panSetPoint);
        break;

      default:
        break;
    }

    // Determine new pitch position
    can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_PITCH_POSITION,
                         pitchPosition);
    pitchPosition = RAD_TO_DEG(pitchPosition);

    // *** UPDATE JOINT SET POINTS ***
    Pan::manager.getActiveController()->setSetPoint((float)panSetPoint);
    Pitch::pitchServo.setValue((float)pitchPosition);

    // *** COMPUTE ACTUATOR CONTROLS ***
    Pan::manager.getActiveController()->update();

    // *** UPDATE TX SIGNALS ***
    can.setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA, HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_POSITION,
                         DEG_TO_RAD(Pan::manager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA,
                         HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Pan::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    // TODO: REPORT FAULTS

    // TODO: REPORT CAN DIAGNOSTICS

    ThisThread::sleep_for(1ms);
  }
}

// *** HANDLERS FOR CAN RX ONE-SHOTS ***

static mbed_error_status_t gimbalSetControlMode(void) {
  bool success = true;
  HWBRIDGE::CANSignalValue_t controlMode;

  success &= can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_CONTROL_MODE,
                                  HWBRIDGE::CANSIGNAL::GIMBAL_PAN_CONTROL_MODE, controlMode) &&
             Pan::manager.switchControlMode((HWBRIDGE::CONTROL::Mode)controlMode);

  if (success) {
    // Send ACK message back
    sendACK(HWBRIDGE::GIMBAL_ACK_VALUES::GIMBAL_ACK_GIMBAL_SET_CONTROL_MODE_ACK);
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t gimbalSetJointPIDParams(void) {
  bool success = true;

  HWBRIDGE::CANSignalValue_t jointID;
  HWBRIDGE::CANSignalValue_t p, i, d, deadzone;

  // Extract signal values
  success &= can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_PID_PARAMS, HWBRIDGE::CANSIGNAL::GIMBAL_JOINT_PIDID,
                                  jointID);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::GIMBAL_JOINT_PID_PROPORTIONAL_GAIN, p);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::GIMBAL_JOINT_PID_INTEGRAL_GAIN, i);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::GIMBAL_JOINT_PID_DERIVATIVE_GAIN, d);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::GIMBAL_JOINT_PID_DEADZONE, deadzone);

  if ((HWBRIDGE::GIMBAL_JOINT_PIDID_VALUES)jointID != HWBRIDGE::GIMBAL_JOINT_PIDID_VALUES::GIMBAL_JOINT_PIDID_PAN) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  if (success) {
    // Set PID params
    if (auto pid = Pan::manager.getActiveController()->getPID()) {
      pid.value().get().updateProportionalGain(p);
      pid.value().get().updateIntegralGain(i);
      pid.value().get().updateDerivativeGain(d);
      pid.value().get().updateDeadzone(deadzone);

      // Send ACK message back
      sendACK(HWBRIDGE::GIMBAL_ACK_VALUES::GIMBAL_ACK_GIMBAL_SET_JOINT_PID_PARAMS_ACK);

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
      can.switchCANBus((HWBRIDGE::CANBUSID)canBusID);

  if (success) {
    // Send ACK message back
    sendACK(HWBRIDGE::GIMBAL_ACK_VALUES::GIMBAL_ACK_CAN_BUS_SWITCH_ACK);
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
