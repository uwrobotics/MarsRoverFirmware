/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CANBus.h"
#include "CANMsg.h"
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
Thread rxCANProcessorThread(osPriorityAboveNormal);

// Incoming message processor
void rxCANProcessor() {
  const auto RX_PERIOD = 2ms;

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

    ThisThread::sleep_for(RX_PERIOD);
  }
}

int main() {
  printf("\r\n\r\n");
  printf("GIMBAL APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  // CAN filter
  can1.setFilter(HWBRIDGE::CANFILTER::ROVER_CANID_FIRST_GIMBAL_RX, CANStandard,
                 HWBRIDGE::ROVERCONFIG::ROVER_CANID_FILTER_MASK);

  rxCANProcessorThread.start(rxCANProcessor);

  while (true) {
    // Output info over serial
    Pan::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}
