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
Thread rxCANProcessorThread(osPriorityAboveNormal);

LEDMatrix matrix(PAN_ENC_SPI_CS, PAN_ENC_SPI_SCK, PAN_ENC_SPI_MISO);

void LEDMatrixHandler(uint8_t color) {
  enum { RED = 0, BLUE = 1, FLASHING_GREEN = 2, OFF = 3 };  // THIS IS WHAT SW SENDS
  switch (color) {
    case RED:
      matrix.setColor(255, 0, 0);
      break;
    case BLUE:
      matrix.setColor(0, 0, 255);
      break;
    case FLASHING_GREEN:
      matrix.flashColor(0, 255, 0);
      break;
    case OFF:
      matrix.clearLights();
      break;
    default:
      break;
  }
}

// Incoming message processor
void rxCANProcessor() {
  const auto RX_PERIOD = 2ms;

  float data = 0;
  HWBRIDGE::CONTROL::Mode controlMode;
  uint8_t color;

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
        case HWBRIDGE::CANID::NEOPIXEL_SET:
          rxMsg.getPayload(color);
          LEDMatrixHandler(color);
          break;
        default:
          break;
      }
    }

    ThisThread::sleep_for(RX_PERIOD);
  }
}

int main() {
  Utility::Logger::printf("\n\n");
  Utility::Logger::printf("GIMBAL APPLICATION STARTED\n");
  Utility::Logger::printf("=======================\n");

  rxCANProcessorThread.start(rxCANProcessor);

  while (true) {
    // Output info over serial
    Pan::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}
