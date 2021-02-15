#include "AdafruitSTEMMA.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "CentrifugeConfig.h"
#include "CoverConfig.h"
#include "DiggerConfig.h"
#include "ElevatorConfig.h"

// TODO: Handle function call failures better

// Felix TODO: Brose through logic and make sure there aren't any silly mistakes.

Sensor::AdafruitSTEMMA moistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL);

CANBus can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

static mbed_error_status_t setMotionData(CANMsg &msg) {
  float motionData;
  msg.getPayload(motionData);

  switch (msg.getID()) {
    case HWBRIDGE::CANID::SET_GENEVA_ANGLE:
      Centrifuge::manager.getActiveController()->setSetPoint(motionData);
      break;
    case HWBRIDGE::CANID::SET_SCOOPER_ANGLE:
      Digger::servo.setValue(motionData);
      break;
    case HWBRIDGE::CANID::SET_COVER_ANGLE:
      Cover::servo.setValue(motionData);
      break;
    case HWBRIDGE::CANID::SET_ELEVATOR_HEIGHT:
      Elevator::manager.getActiveController()->setSetPoint(motionData);
      break;
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
  return MBED_SUCCESS;
}

static CANMsg::CANMsgHandlerMap canHandlerMap = {{HWBRIDGE::CANID::SET_GENEVA_ANGLE, &setMotionData},
                                                 {HWBRIDGE::CANID::SET_SCOOPER_ANGLE, &setMotionData},
                                                 {HWBRIDGE::CANID::SET_COVER_ANGLE, &setMotionData},
                                                 {HWBRIDGE::CANID::SET_ELEVATOR_HEIGHT, &setMotionData}};

Thread rxCANProcessorThread;
Thread txCANProcessorThread;

void rxCANProcessor() {
  CANMsg rxMsg;
  while (true) {
    if (can.read(rxMsg)) {
      canHandlerMap.at(rxMsg.getID())(rxMsg);
    }
  }
}

void txCANProcessor() {
  constexpr auto txPeriod     = 500ms;
  constexpr auto txInterdelay = 2ms;
  CANMsg txMsg;

  struct __attribute__((__packed__)) MotionReport {
    float position = 0, velocity = 0;
  };

  // Felix TODO: Ensure that each time, we get the right data, call setPayload with new data and send the new data
  while (true) {
    MotionReport report;
    float angle = 0, speed = 0;

    txMsg.setID(HWBRIDGE::CANID::REPORT_GENEVA_ANGLE);
    Centrifuge::manager.getActiveController()->reportAngleDeg(angle);
    Centrifuge::manager.getActiveController()->reportAngularVelocityDegPerSec(speed);
    report = {angle, speed};
    txMsg.setPayload(report);
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_ELEVATOR_HEIGHT);
    Elevator::manager.getActiveController()->reportAngleDeg(angle);
    Elevator::manager.getActiveController()->reportAngularVelocityDegPerSec(speed);
    report = {angle, speed};
    txMsg.setPayload(report);
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    // Read moisture returns an unsigned number so it needs to be cast to an int to be handled
    txMsg.setID(HWBRIDGE::CANID::REPORT_MOISTURE_DATA);
    float moisture = 0;
    moistureSensor.read(moisture);
    txMsg.setPayload(moisture);
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_TEMPERATURE_DATA);
    float temperature = 0;
    moistureSensor.alternateRead(temperature);
    txMsg.setPayload(temperature);
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    ThisThread::sleep_for(txPeriod);
  }
}

int main() {
  printf("\r\n\r\n");
  printf("SCIENCE APP STARTED!\r\n");
  printf("====================\r\n");

  rxCANProcessorThread.start(rxCANProcessor);
  txCANProcessorThread.start(txCANProcessor);

  while (true) {
    Centrifuge::manager.getActiveController()->update();
    Elevator::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}
