#include "AdafruitSTEMMA.h"
#include "CANConfig.h"
#include "CANMsg.h"
#include "CentrifugeConfig.h"
#include "CoverConfig.h"
#include "DiggerConfig.h"
#include "ElevatorConfig.h"
#include "Logger.h"

Sensor::AdafruitSTEMMA moistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL);

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

  HWBRIDGE::CANSignalValue_t genevaSetPoint;
  HWBRIDGE::CANSignalValue_t elevatorSetPoint;
  HWBRIDGE::CANSignalValue_t coverPosition;
  HWBRIDGE::CANSignalValue_t shovelPosition;

  printf("\r\n\r\n");
  printf("SCIENCE APP STARTED!\r\n");
  printf("====================\r\n");

  // Set CAN filters
  can.setFilter(HWBRIDGE::CANFILTER::SCIENCE_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  while (true) {
    // *** PROCESS CAN RX SIGNALS (TODO: NEED TO HANDLE SNA CASES) ***

    // Determine new geneva mechanism position
    switch (Centrifuge::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION,
                               HWBRIDGE::CANSIGNAL::SCIENCE_SET_GENEVA_POSITION, genevaSetPoint);
        genevaSetPoint = RAD_TO_DEG(genevaSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::SCIENCE_SET_GENEVA_ANGULAR_VELOCITY, genevaSetPoint);
        genevaSetPoint = RAD_TO_DEG(genevaSetPoint);
        break;

      default:
        break;
    }

    // Determine new elevator position
    switch (Elevator::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION,
                               HWBRIDGE::CANSIGNAL::SCIENCE_SET_ELEVATOR_POSITION, elevatorSetPoint);
        elevatorSetPoint = RAD_TO_DEG(elevatorSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_ANGULAR_VELOCITY,
                               HWBRIDGE::CANSIGNAL::SCIENCE_SET_ELEVATOR_ANGULAR_VELOCITY, elevatorSetPoint);
        elevatorSetPoint = RAD_TO_DEG(elevatorSetPoint);
        break;

      default:
        break;
    }

    // Determine new cover position
    can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::SCIENCE_SET_COVER_POSITION,
                           coverPosition);
    coverPosition = RAD_TO_DEG(coverPosition);

    // Determine new shovel position
    can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION,
                           HWBRIDGE::CANSIGNAL::SCIENCE_SET_SHOVEL_POSITION, shovelPosition);
    shovelPosition = RAD_TO_DEG(shovelPosition);

    // *** UPDATE JOINT SET POINTS ***
    Centrifuge::manager.getActiveController()->setSetPoint((float)genevaSetPoint);
    Elevator::manager.getActiveController()->setSetPoint((float)elevatorSetPoint);
    Cover::servo.setValue((float)coverPosition);
    Digger::servo.setValue((float)shovelPosition);

    // *** COMPUTE ACTUATOR CONTROLS ***
    Centrifuge::manager.getActiveController()->update();
    Elevator::manager.getActiveController()->update();

    // *** UPDATE TX SIGNALS ***

    // Joint position
    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                             HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_GENEVA_POSITION,
                             DEG_TO_RAD(Centrifuge::manager.getActiveController()->reportAngleDeg()));

    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                             HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_ELEVATOR_POSITION,
                             DEG_TO_RAD(Elevator::manager.getActiveController()->reportAngleDeg()));

    // Joint angular velocity
    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                             HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_GENEVA_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Centrifuge::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                             HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_ELEVATOR_ANGULAR_VELOCITY,
                             DEG_TO_RAD(Elevator::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    // Sensor data
    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_SENSOR_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_MOISTURE_DATA,
                             moistureSensor.read());

    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_SENSOR_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_TEMPERATURE_DATA,
                             moistureSensor.alternateRead());

    // TODO: REPORT FAULTS

    // Print CAN diagnostics
    printf("SCIENCE - number of CAN messages received: %lu\r\n", can.getNumMsgsReceived());

    ThisThread::sleep_for(1ms);
  }
}

// *** HANDLERS FOR CAN RX ONE-SHOTS ***

static mbed_error_status_t scienceSetControlMode(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::SCIENCE_SET_CONTROL_MODE) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CONTROL::Mode controlMode;
  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_science_set_control_mode_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_science_set_control_mode_unpack(&msgStruct, msgData.raw,
                                                          UWRT_MARS_ROVER_CAN_SCIENCE_SET_CONTROL_MODE_LENGTH) == 0) {
    // Set geneva control mode
    controlMode =
        (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_science_set_control_mode_science_geneva_control_mode_decode(
            msgStruct.science_geneva_control_mode);
    success &= Centrifuge::manager.switchControlMode(controlMode);

    // Set elevator control mode
    controlMode =
        (HWBRIDGE::CONTROL::Mode)uwrt_mars_rover_can_science_set_control_mode_science_elevator_control_mode_decode(
            msgStruct.science_elevator_control_mode);
    success &= Elevator::manager.switchControlMode(controlMode);

    if (success) {
      // Send ACK message back
      sendACK(HWBRIDGE::SCIENCE_ACK_VALUES::SCIENCE_ACK_SCIENCE_SET_CONTROL_MODE_ACK);
    }
  } else {
    // Error unpacking!
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t scienceSetJointPIDParams(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::SCIENCE_SET_JOINT_PID_PARAMS) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_science_set_joint_pid_params_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_science_set_joint_pid_params_unpack(
          &msgStruct, msgData.raw, UWRT_MARS_ROVER_CAN_SCIENCE_SET_JOINT_PID_PARAMS_LENGTH) == 0) {
    // Error check joint ID
    HWBRIDGE::SCIENCE_JOINT_PIDID_VALUES jointID = (HWBRIDGE::SCIENCE_JOINT_PIDID_VALUES)
        uwrt_mars_rover_can_science_set_joint_pid_params_science_joint_pidid_decode(msgStruct.science_joint_pidid);

    Controller::ActuatorControllerManager* act;

    switch (jointID) {
      case HWBRIDGE::SCIENCE_JOINT_PIDID_VALUES::SCIENCE_JOINT_PIDID_GENEVA:
        act = &Centrifuge::manager;
        break;

      case HWBRIDGE::SCIENCE_JOINT_PIDID_VALUES::SCIENCE_JOINT_PIDID_ELEVATOR:
        act = &Elevator::manager;
        break;

      default:
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Extract PID params
    float p = (float)uwrt_mars_rover_can_science_set_joint_pid_params_science_joint_pid_proportional_gain_decode(
        msgStruct.science_joint_pid_proportional_gain);
    float i = (float)uwrt_mars_rover_can_science_set_joint_pid_params_science_joint_pid_integral_gain_decode(
        msgStruct.science_joint_pid_integral_gain);
    float d = (float)uwrt_mars_rover_can_science_set_joint_pid_params_science_joint_pid_derivative_gain_decode(
        msgStruct.science_joint_pid_derivative_gain);
    float deadzone = (float)uwrt_mars_rover_can_science_set_joint_pid_params_science_joint_pid_deadzone_decode(
        msgStruct.science_joint_pid_proportional_gain);

    // Set PID params
    if (auto pid = act->getActiveController()->getPID()) {
      if (msgStruct.science_joint_pid_proportional_gain !=
          (uint16_t)HWBRIDGE::SCIENCE_JOINT_PID_PROPORTIONAL_GAIN_VALUES::SCIENCE_JOINT_PID_PROPORTIONAL_GAIN_SNA) {
        pid.value().get().updateProportionalGain(p);
      }
      if (msgStruct.science_joint_pid_integral_gain !=
          (uint16_t)HWBRIDGE::SCIENCE_JOINT_PID_INTEGRAL_GAIN_VALUES::SCIENCE_JOINT_PID_INTEGRAL_GAIN_SNA) {
        pid.value().get().updateIntegralGain(i);
      }
      if (msgStruct.science_joint_pid_derivative_gain !=
          (uint16_t)HWBRIDGE::SCIENCE_JOINT_PID_DERIVATIVE_GAIN_VALUES::SCIENCE_JOINT_PID_DERIVATIVE_GAIN_SNA) {
        pid.value().get().updateDerivativeGain(d);
      }
      if (msgStruct.science_joint_pid_deadzone !=
          (uint16_t)HWBRIDGE::SCIENCE_JOINT_PID_DEADZONE_VALUES::SCIENCE_JOINT_PID_DEADZONE_SNA) {
        pid.value().get().updateDeadzone(deadzone);
      }

      // Send ACK message back
      sendACK(HWBRIDGE::SCIENCE_ACK_VALUES::SCIENCE_ACK_SCIENCE_SET_JOINT_PID_PARAMS_ACK);

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
    sendACK(HWBRIDGE::SCIENCE_ACK_VALUES::SCIENCE_ACK_CAN_BUS_SWITCH_ACK);
  } else {
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static void sendACK(HWBRIDGE::SCIENCE_ACK_VALUES ackValue) {
  struct uwrt_mars_rover_can_science_report_ack_t ackMsgStruct = {
      .science_ack = (uint8_t)ackValue,
  };

  HWBRIDGE::CANMsgData_t ackMsgData;
  uwrt_mars_rover_can_science_report_ack_pack(ackMsgData.raw, &ackMsgStruct,
                                              UWRT_MARS_ROVER_CAN_SCIENCE_REPORT_ACK_LENGTH);

  CANMsg msgACK;
  msgACK.setID(HWBRIDGE::CANID::SCIENCE_REPORT_ACK);
  msgACK.setPayload(ackMsgData, UWRT_MARS_ROVER_CAN_SCIENCE_REPORT_ACK_LENGTH);

  can.sendOneShotMessage(msgACK, 1ms);
}
