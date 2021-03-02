/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CANBus.h"
#include "CANMsg.h"
#include "LEDMatrix.h"
#include "Logger.h"
#include "PanConfig.h"
#include "PinNames.h"
#include "PitchConfig.h"
#include "RollConfig.h"
#include "hw_bridge.h"

// Init. Components
// CAN Object
CANBus can1(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
CANMsg rxMsg;

// Threads
Thread rxCANPostmanThread(osPriorityRealtime);
Thread rxCANClientThread(osPriorityAboveNormal);

// CAN Processing Objects
Mail<CANMsg, 100> mail_box;
EventQueue event_queue;

LEDMatrix matrix(PAN_ENC_SPI_CS, PAN_ENC_SPI_SCK, PAN_ENC_SPI_MISO);

void LEDMatrixHandler(uint8_t color) {
  enum { RED = 0, BLUE = 1, FLASHING_GREEN = 2, OFF = 3 };  // THIS IS WHAT SW SENDS
  switch (color) {
    case RED:
      matrix.setColor(RED);
      break;
    case BLUE:
      matrix.setColor(BLUE);
      break;
    case FLASHING_GREEN:
      matrix.flashGreen(); // THIS WILL FLASH FOR 10 SECONDS AND WONT STOP TIL ITS DONE.
      break;
    case OFF:
      matrix.clearLights();
      break;
    default:
      break;
  }
}

// Incoming message processor
void rxCANConsumer(CANMsg &rxMsg) {
  const auto rxPeriod = 2ms;

  float data = 0;
  HWBRIDGE::CONTROL::Mode controlMode;
  uint8_t color;

  switch (rxMsg.getID()) {
    case HWBRIDGE::CANID::SET_PAN_MOTION_DATA:
      rxMsg.getPayload(data);
      Pan::manager.getActiveController()->setSetPoint(data);
      break;
    case HWBRIDGE::CANID::SET_PITCH_MOTION_DATA:
      rxMsg.getPayload(data);
      Pitch::pitchServo.setValue(data);
      break;
    case HWBRIDGE::CANID::SET_ROLL_MOTION_DATA:
      rxMsg.getPayload(data);
      Roll::rollServo.setValue(data);
      break;
    case HWBRIDGE::CANID::SET_PAN_CONTROL_MODE:
      rxMsg.getPayload(controlMode);
      Pan::manager.switchControlMode(controlMode);
      break;
    case HWBRIDGE::CANID::NEOPIXEL_SET:
      rxMsg.getPayload(color);
      LEDMatrixHandler(color);
      break;
    default:
      break;
  }
}

void rxCANClient() {
  while (true) {
    CANMsg *mail = nullptr;
    do {
      mail = mail_box.try_get();  // TODO: try_get_for was not working. Investigate why and use it
      ThisThread::sleep_for(1ms);
    } while (mail == nullptr);
    MBED_ASSERT(mail != nullptr);
    rxCANConsumer(*mail);
    MBED_ASSERT(mail_box.free(mail) == osOK);
  }
}

// this function is indirectly triggered by an IRQ. It reads a CAN msg and puts in the mail_box
void rxCANPostman() {
  CANMsg msg;
  // this loop is needed to avoid missing msg received between turning off the IRQ and turning it back on
  while (can1.read(msg)) {
    // TODO: Handle mail related errors better
    CANMsg *mail = mail_box.try_alloc_for(1ms);
    MBED_ASSERT(mail != nullptr);
    *mail = msg;
    mail_box.put(mail);
  }
  can_irq_set(can1.getHandle(), IRQ_RX, true);
}

void rxCANISR() {
  can_irq_set(can1.getHandle(), IRQ_RX, false);
  event_queue.call(&rxCANPostman);
}

int main() {
  Utility::Logger::printf("\r\n\r\n");
  Utility::Logger::printf("GIMBAL APPLICATION STARTED\r\n");
  Utility::Logger::printf("=======================\r\n");

  rxCANPostmanThread.start(callback(&event_queue, &EventQueue::dispatch_forever));
  rxCANClientThread.start(&rxCANClient);

  can1.attach(&rxCANISR, CANBus::RxIrq);

  while (true) {
    Pan::manager.getActiveController()->update();
    ThisThread::sleep_for(1ms);
  }
}
