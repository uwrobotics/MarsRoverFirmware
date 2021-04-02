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
  HWBRIDGE::CANSignalValue_t genevaSetPoint;
  HWBRIDGE::CANSignalValue_t elevatorSetPoint;
  HWBRIDGE::CANSignalValue_t coverPosition;
  HWBRIDGE::CANSignalValue_t shovelPosition;

  printf("\r\n\r\n");
  printf("SCIENCE APP STARTED!\r\n");
  printf("====================\r\n");

  // Set CAN filters
  can.setFilter(HWBRIDGE::CANFILTER::SCIENCE_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 0);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 1);

  while (true) {
    // Process CAN RX signals (TODO: NEED TO HANDLE SNA CASES)
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

    can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::SCIENCE_SET_COVER_POSITION,
                           coverPosition);
    coverPosition = RAD_TO_DEG(coverPosition);
    can.readStreamedSignal(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION,
                           HWBRIDGE::CANSIGNAL::SCIENCE_SET_SHOVEL_POSITION, shovelPosition);
    shovelPosition = RAD_TO_DEG(shovelPosition);

    // Update joint set points
    Centrifuge::manager.getActiveController()->setSetPoint((float)genevaSetPoint);
    Elevator::manager.getActiveController()->setSetPoint((float)elevatorSetPoint);
    Cover::servo.setValue((float)coverPosition);
    Digger::servo.setValue((float)shovelPosition);

    // Compute actuator controls
    Centrifuge::manager.getActiveController()->update();
    Elevator::manager.getActiveController()->update();

    // Update TX signals
    can.updateStreamedSignal(
        HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_GENEVA_POSITION,
        (HWBRIDGE::CANSignalValue_t)DEG_TO_RAD(Centrifuge::manager.getActiveController()->reportAngleDeg()));
    can.updateStreamedSignal(
        HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_ELEVATOR_POSITION,
        (HWBRIDGE::CANSignalValue_t)DEG_TO_RAD(Elevator::manager.getActiveController()->reportAngleDeg()));
    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                             HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_GENEVA_ANGULAR_VELOCITY,
                             (HWBRIDGE::CANSignalValue_t)DEG_TO_RAD(
                                 Centrifuge::manager.getActiveController()->reportAngularVelocityDegPerSec()));
    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                             HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_ELEVATOR_ANGULAR_VELOCITY,
                             (HWBRIDGE::CANSignalValue_t)DEG_TO_RAD(
                                 Elevator::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_SENSOR_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_MOISTURE_DATA,
                             (HWBRIDGE::CANSignalValue_t)moistureSensor.read());
    can.updateStreamedSignal(HWBRIDGE::CANID::SCIENCE_REPORT_SENSOR_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_TEMPERATURE_DATA,
                             (HWBRIDGE::CANSignalValue_t)moistureSensor.alternateRead());

    // TODO: REPORT FAULTS

    ThisThread::sleep_for(1ms);
  }
}

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
