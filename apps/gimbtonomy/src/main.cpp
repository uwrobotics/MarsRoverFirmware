/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CANBus.h"
#include "CANMsg.h"
#include "Logger.h"
#include "Neopixel_Blocking.h"
#include "PanConfig.h"
#include "PitchConfig.h"
#include "RollConfig.h"
#include "hw_bridge.h"

// Orson TODO: rename everything that says gimbtonomy to gimbal. rename files, folders, cmake pckgs etc

// Orson TODO: remove this debug functionality. (remove anythign that uses this debug macro)
#define DEBUG

constexpr uint8_t ACK_FLAG(1UL << 0);

// Init. Components
// Servos
/*
• Pan: HS-1425 CR (https://www.robotshop.com/en/hitec-hsr-1425cr-continuous-rotation-servo.html)
• Pitch: HS-422 (https://www.robotshop.com/en/hitec-hs-422-servo-motor.html)
• Roll: SG90 (https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1)
*/
// CAN Object
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// neopixel
// Orson TODO: removing any mention of neopixel from Gimbal. (it has been moved to pdb)
Neopixel_Blocking neopixel(16, LED_MTRX);

// Threads
Thread rxCANPostmanThread(osPriorityRealtime);
Thread rxCANClientThread(osPriorityAboveNormal);
Thread txCANProcessorThread(osPriorityBelowNormal);

// CAN Processing Objects
Mail<CANMsg, 100> mail_box;
EventQueue event_queue;

// Event flags for communication between threads
EventFlags event_flags;

// Orson TODO: carefully go through each function and make sure there arent any silly mistakes. like if the canid says
// set motion of pan servo make sure it actually does that
void handleSetNeoPixelColor(CANMsg *p_newMsg) {
  enum mode : uint8_t { solidRed, solidBlue, flashingGreen, off };
  uint8_t neoPixelMode = 0;
  *p_newMsg >> neoPixelMode;
  switch (neoPixelMode) {
    case solidRed:
      Utility::Logger::printf("Setting neo pixels to solid red\r\n");
      neopixel.displayRed();
      break;
    case solidBlue:
      Utility::Logger::printf("Setting neo pixels to solid blue\r\n");
      neopixel.displayBlue();
      break;
    case flashingGreen:
      Utility::Logger::printf("Setting neo pixels to flashing green\r\n");
      neopixel.blinkPixels(10, 2s, neopixel.Green);
      break;
    case off:
      Utility::Logger::printf("Turning neo pixels off\r\n");
      neopixel.shutdown();
      break;
    default:
      Utility::Logger::printf("Neo pixels tried to be set to unknow mode\r\n");
      break;
  }
}

// Incoming message processor
void rxCANConsumer(CANMsg &rxMsg) {
  const auto rxPeriod = 2ms;

  float data = 0;
  HWBRIDGE::CONTROL::Mode controlMode;

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
      handleSetNeoPixelColor(&rxMsg);
      // Hangle can writes in a separate thread
      event_flags.set(ACK_FLAG);
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

// Outgoing message processor
void txCANProcessor() {
  CANMsg txMsg;
  while (true) {
    // This thread does not sleep, but only executes if signaled by rxCANParser to write out a response
    event_flags.wait_any(ACK_FLAG);

    // Send an acknowledgement CANMsg back to the Jetson
#ifdef DEBUG
    Utility::Logger::printf("Sending neopixel acknowledgement message\r\n");
#endif
    txMsg.clear();
    txMsg.setID(HWBRIDGE::CANID::NEOPIXEL_ACK);
    txMsg << true;
    can1.write(txMsg);
#ifdef DEBUG
    Utility::Logger::printf("Updating neo pixels\r\n");
#endif
  }
}

int main() {
  Utility::Logger::printf("\r\n\r\n");
  Utility::Logger::printf("ARM APPLICATION STARTED\r\n");
  Utility::Logger::printf("=======================\r\n");

  rxCANPostmanThread.start(callback(&event_queue, &EventQueue::dispatch_forever));
  rxCANClientThread.start(&rxCANClient);
  txCANProcessorThread.start(&txCANProcessor);

  can1.attach(&rxCANISR, CANBus::RxIrq);

  while (true) {
    // Output info over serial
#ifdef DEBUG
    float speed = 0;
    if (Pan::manager.getActiveController()->reportAngularVelocityDegPerSec(speed)) {
      Utility::Logger::printf("panServo(speed): %f \r\n", speed);
    } else {
      Utility::Logger::printf("panServo(speed): READ ERROR \r\n");
    }
#endif
    Pan::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}
