/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CANBus.h"
#include "CANMsg.h"
#include "Neopixel_Blocking.h"
#include "PanConfig.h"
#include "PitchConfig.h"
#include "RollConfig.h"
#include "hw_bridge.h"

constexpr uint8_t ACK_FLAG(1UL << 0);

// Init. Components
// Servos
/*
• Pan: HS-1425 CR (https://www.robotshop.com/en/hitec-hsr-1425cr-continuous-rotation-servo.html)
• Pitch: HS-422 (https://www.robotshop.com/en/hitec-hs-422-servo-motor.html)
• Roll: SG90 (https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1)
*/
// CAN Object
CANBus can1(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
CANMsg rxMsg, txMsg;

// Threads
Thread rxCANProcessorThread(osPriorityAboveNormal);
Thread txCANProcessorThread(osPriorityBelowNormal);

// Event flags for communication between threads
EventFlags event_flags;

// Incoming message processor
void rxCANProcessor() {
  const auto rxPeriod = 2ms;

  float data = 0;
  HWBRIDGE::CONTROL::Mode controlMode;

  while (true) {
    if (can1.read(rxMsg)) {
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
        default:
          break;
      }
    }

    ThisThread::sleep_for(rxPeriod);
  }
}

// Outgoing message processor
void txCANProcessor() {
  while (true) {
    // This thread does not sleep, but only executes if signaled by rxCANProcessor to write out a response
    event_flags.wait_any(ACK_FLAG);

    // Send an acknowledgement CANMsg back to the Jetson
    txMsg.clear();
    txMsg << true;
    can1.write(txMsg);
  }
}

int main() {
  printf("\r\n\r\n");
  printf("ARM APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  rxCANProcessorThread.start(rxCANProcessor);
  txCANProcessorThread.start(txCANProcessor);

  while (true) {
    // Output info over serial
    Pan::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}
