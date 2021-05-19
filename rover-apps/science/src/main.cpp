#include "AdafruitSTEMMA.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "CentrifugeConfig.h"
#include "CoverConfig.h"
#include "DiggerConfig.h"
#include "ElevatorConfig.h"
#include "Logger.h"

// TODO: Handle function call failures better

// Felix TODO: Brose through logic and make sure there aren't any silly mistakes.

Sensor::AdafruitSTEMMA moistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL);

CANBus can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// CAN Processing Objects
Mail<CANMsg, 100> mail_box;
EventQueue event_queue;

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

Thread rxCANPostmanThread(osPriorityRealtime);
Thread rxCANClientThread(osPriorityAboveNormal);
Thread txCANProcessorThread(osPriorityBelowNormal);

void rxCANClient() {
  while (true) {
    CANMsg *mail = nullptr;
    do {
      mail = mail_box.try_get();  // TODO: try_get_for was not working. Investigate why and use it
      ThisThread::sleep_for(1ms);
    } while (mail == nullptr);
    MBED_ASSERT(mail != nullptr);
    canHandlerMap.at(mail->getID())(*mail);
    MBED_ASSERT(mail_box.free(mail) == osOK);
  }
}

// this function is indirectly triggered by an IRQ. It reads a CAN msg and puts in the mail_box
void rxCANPostman() {
  CANMsg msg;
  // this loop is needed to avoid missing msg received between turning off the IRQ and turning it back on
  while (can.read(msg)) {
    // TODO: Handle mail related errors better
    CANMsg *mail = mail_box.try_alloc_for(1ms);
    MBED_ASSERT(mail != nullptr);
    *mail = msg;
    mail_box.put(mail);
  }
  can_irq_set(can.getHandle(), IRQ_RX, true);
}

void rxCANISR() {
  can_irq_set(can.getHandle(), IRQ_RX, false);
  event_queue.call(&rxCANPostman);
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

    txMsg.setID(HWBRIDGE::CANID::REPORT_GENEVA_ANGLE);
    report = {Centrifuge::manager.getActiveController()->reportAngleDeg(),
              Centrifuge::manager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_ELEVATOR_HEIGHT);
    report = {Elevator::manager.getActiveController()->reportAngleDeg(),
              Elevator::manager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_MOISTURE_DATA);
    txMsg.setPayload(moistureSensor.read());
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_TEMPERATURE_DATA);
    moistureSensor.alternateRead();
    txMsg.setPayload(moistureSensor.alternateRead());
    can.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    ThisThread::sleep_for(txPeriod);
  }
}

int main() {
  printf("\r\n\r\n");
  printf("SCIENCE APP STARTED!\r\n");
  printf("====================\r\n");

  // CAN filter
  can.setFilter(HWBRIDGE::CANFILTER::ROVER_CANID_FIRST_SCIENCE_RX, CANStandard,
                HWBRIDGE::ROVERCONFIG::ROVER_CANID_FILTER_MASK);

  rxCANPostmanThread.start(callback(&event_queue, &EventQueue::dispatch_forever));
  rxCANClientThread.start(&rxCANClient);
  txCANProcessorThread.start(txCANProcessor);

  can.attach(&rxCANISR, CANBus::RxIrq);

  while (true) {
    Centrifuge::manager.getActiveController()->update();
    Elevator::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}
