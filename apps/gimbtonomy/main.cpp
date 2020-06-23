/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CANMsg.h"
#include "mbed.h"
#include "Neopixel_Blocking.h"
#include "hw_bridge.h"

CAN can(CAN1_RX, CAN1_TX, ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
CANMsg rxMsg, txMsg;
// CAN_RX = PB_8, CAN_TX = PB_9
Neopixel_Blocking neopixel(16, LED_MTRX);

// Color is specified by the data inside the packet

void initCAN() {
  can.filter(ROVERCONFIG::ROVER_CANID_FIRST_GIMBTONOMY_RX, ROVERCONFIG::ROVER_CANID_FILTER_MASK, CANStandard);
}

void handleSetNeoPixelColor(CANMsg *p_newMsg) {
  enum mode : uint8_t { solidRed, solidBlue, flashingGreen, off };
  uint8_t neoPixelMode = 0;
  *p_newMsg >> neoPixelMode;
  switch (neoPixelMode) {
    case solidRed:
      printf("Setting neo pixels to solid red\r\n");
      neopixel.displayRed();
      break;
    case solidBlue:
      printf("Setting neo pixels to solid blue\r\n");
      neopixel.displayBlue();
      break;
    case flashingGreen:
      printf("Setting neo pixels to flashing green\r\n");
      neopixel.blinkPixels(10, 2, neopixel.Green);
      break;
    case off:
      printf("Turning neo pixels off\r\n");
      neopixel.shutdown();
      break;
    default:
      printf("Neo pixels tried to be set to unknow mode\r\n");
      break;
  }
}

void processCANMsg(CANMsg *p_newMsg) {
  switch (p_newMsg->id) {
    case CANID::SET_NEOPIXEL:
      // Send an acknowledgement CANMsg back to the Jetson
      printf("Sending acknowledgement message\r\n");
      txMsg.clear();
      txMsg.id = CANID::NEOPIXEL_ACK;
      txMsg << true;
      can.write(txMsg);
      printf("Updating neo pixels\r\n");
      handleSetNeoPixelColor(p_newMsg);
      break;
    default:
      printf("Recieved unimplemented command for neopixels\r\n");
      break;
  }
}

// main() runs in its own thread in the OS
int main() {
  printf("Beginning neopixel fw app.\r\n");
  initCAN();
  while (1) {
    if (can.read(rxMsg)) {
      processCANMsg(&rxMsg);
      rxMsg.clear();
    }
  }
  return 1;
}
