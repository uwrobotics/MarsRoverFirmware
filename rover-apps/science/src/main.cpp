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
  can.setFilter(HWBRIDGE::CANFILTER::SCIENCE_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 0);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 1);

  while (true) {
    // *** PROCESS CAN RX SIGNALS ***

    // Determine new geneva mechanism position
    switch (Centrifuge::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION,
                             HWBRIDGE::CANSIGNAL::SCIENCE_SET_GENEVA_POSITION, genevaSetPoint);
        genevaSetPoint = RAD_TO_DEG(genevaSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_ANGULAR_VELOCITY,
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
        can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION,
                             HWBRIDGE::CANSIGNAL::SCIENCE_SET_ELEVATOR_POSITION, elevatorSetPoint);
        elevatorSetPoint = RAD_TO_DEG(elevatorSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::SCIENCE_SET_ELEVATOR_ANGULAR_VELOCITY, elevatorSetPoint);
        elevatorSetPoint = RAD_TO_DEG(elevatorSetPoint);
        break;

      default:
        break;
    }

    // Determine new cover position
    can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::SCIENCE_SET_COVER_POSITION,
                         coverPosition);
    coverPosition = RAD_TO_DEG(coverPosition);

    // Determine new shovel position
    can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::SCIENCE_SET_SHOVEL_POSITION,
                         shovelPosition);
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
    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                         HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_GENEVA_POSITION,
                         DEG_TO_RAD(Centrifuge::manager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                         HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_ELEVATOR_POSITION,
                         DEG_TO_RAD(Elevator::manager.getActiveController()->reportAngleDeg()));

    // Joint angular velocity
    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                         HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_GENEVA_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Centrifuge::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_JOINT_DATA,
                         HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_ELEVATOR_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Elevator::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    // Sensor data
    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_SENSOR_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_MOISTURE_DATA,
                         moistureSensor.read());

    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_SENSOR_DATA, HWBRIDGE::CANSIGNAL::SCIENCE_TEMPERATURE_DATA,
                         moistureSensor.alternateRead());

    // TODO: REPORT FAULTS
    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::SCIENCE_NUM_CANRX_FAULTS,
                         can.getNumCANRXFaults());
    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::SCIENCE_NUM_CANTX_FAULTS,
                         can.getNumCANTXFaults());

    // Report diagnostics
    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_DIAGNOSTICS,
                         HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_NUM_STREAMED_MSGS_RECEIVED,
                         can.getNumStreamedMsgsReceived());
    can.setTXSignalValue(HWBRIDGE::CANID::SCIENCE_REPORT_DIAGNOSTICS,
                         HWBRIDGE::CANSIGNAL::SCIENCE_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED,
                         can.getNumOneShotMsgsReceived());

    ThisThread::sleep_for(1ms);
  }
}

// *** HANDLERS FOR CAN RX ONE-SHOTS ***

static mbed_error_status_t scienceSetControlMode(void) {
  bool success = true;
  HWBRIDGE::CANSignalValue_t controlMode;

  // Set geneva mechanism control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_CONTROL_MODE, HWBRIDGE::CANSIGNAL::SCIENCE_GENEVA_CONTROL_MODE,
                           controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Centrifuge::manager.getActiveControlMode()) {
      success &= Centrifuge::manager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  // Set elevator control mode
  if (can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_CONTROL_MODE,
                           HWBRIDGE::CANSIGNAL::SCIENCE_ELEVATOR_CONTROL_MODE, controlMode)) {
    if (static_cast<HWBRIDGE::CONTROL::Mode>(controlMode) != Elevator::manager.getActiveControlMode()) {
      success &= Elevator::manager.switchControlMode(static_cast<HWBRIDGE::CONTROL::Mode>(controlMode));
    }
  } else {
    success = false;
  }

  if (success) {
    // Send ACK message back
    sendACK(HWBRIDGE::SCIENCE_ACK_VALUES::SCIENCE_ACK_SCIENCE_SET_CONTROL_MODE_ACK);
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t scienceSetJointPIDParams(void) {
  bool success = true;

  HWBRIDGE::CANSignalValue_t jointID;
  HWBRIDGE::CANSignalValue_t p, i, d, deadzone;

  // Extract signal values
  success &= can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::SCIENCE_JOINT_PIDID, jointID);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::SCIENCE_JOINT_PID_PROPORTIONAL_GAIN, p);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::SCIENCE_JOINT_PID_INTEGRAL_GAIN, i);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::SCIENCE_JOINT_PID_DERIVATIVE_GAIN, d);
  success &= can.getRXSignalValue(HWBRIDGE::CANID::SCIENCE_SET_JOINT_PID_PARAMS,
                                  HWBRIDGE::CANSIGNAL::SCIENCE_JOINT_PID_DEADZONE, deadzone);

  if (success) {
    Controller::ActuatorControllerManager* act;

    switch (static_cast<HWBRIDGE::SCIENCE_JOINT_PIDID_VALUES>(jointID)) {
      case HWBRIDGE::SCIENCE_JOINT_PIDID_VALUES::SCIENCE_JOINT_PIDID_GENEVA:
        act = &Centrifuge::manager;
        break;

      case HWBRIDGE::SCIENCE_JOINT_PIDID_VALUES::SCIENCE_JOINT_PIDID_ELEVATOR:
        act = &Elevator::manager;
        break;

      default:
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Set PID params
    if (auto pid = act->getActiveController()->getPID()) {
      pid.value().get().updateProportionalGain(p);
      pid.value().get().updateIntegralGain(i);
      pid.value().get().updateDerivativeGain(d);
      pid.value().get().updateDeadzone(deadzone);

      // Send ACK message back
      sendACK(HWBRIDGE::SCIENCE_ACK_VALUES::SCIENCE_ACK_SCIENCE_SET_JOINT_PID_PARAMS_ACK);
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
    sendACK(HWBRIDGE::SCIENCE_ACK_VALUES::SCIENCE_ACK_CAN_BUS_SWITCH_ACK);
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static void sendACK(HWBRIDGE::SCIENCE_ACK_VALUES ackValue) {
  struct uwrt_mars_rover_can_science_report_ack_t ackMsgStruct = {
      .science_ack = static_cast<uint8_t>(ackValue),
  };

  HWBRIDGE::CANMsgData_t ackMsgData;
  uwrt_mars_rover_can_science_report_ack_pack(ackMsgData.raw, &ackMsgStruct,
                                              UWRT_MARS_ROVER_CAN_SCIENCE_REPORT_ACK_LENGTH);

  CANMsg msgACK;
  msgACK.setID(HWBRIDGE::CANID::SCIENCE_REPORT_ACK);
  msgACK.setPayload(ackMsgData, UWRT_MARS_ROVER_CAN_SCIENCE_REPORT_ACK_LENGTH);

  can.sendOneShotMessage(msgACK, 1ms);
}
