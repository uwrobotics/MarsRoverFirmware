/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "CANMsg.h"
#include "rover_config.h"
#include "blockingneopixel.h"

Serial        pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);
// CAN_RX = PB_8, CAN_TX = PB_9
CAN           can(PB_8, PB_9, ROVER_CANBUS_FREQUENCY);
CANMsg        rxMsg;
BlockingNeopixel      neopixel(16);

// 0x794 CAN ID for change to a neo pixel.
// Color is specified by the data inside the packet
// 0 is solid red
// 1 is solid blue
// 2 is flashing green

void initCAN() {
    // CANStandard is defined in CAN.h
    can.filter(ROVER_CANID_FIRST_GIMBTONOMY_RX, ROVER_CANID_FILTER_MASK, CANStandard);
}

void handleSetNeoPixelColor(CANMsg *p_newMsg){
    // 0 = solid red, 1 = solid blue, 2 = flashing green
    unsigned int neoPixelMode = 0;
    *p_newMsg >> neoPixelMode;

    switch (neoPixelMode){
    case 0:
        pc.printf("Setting neo pixels to solid red\r\n");
        neopixel.displayRed();
        break;
    case 1:
        pc.printf("Setting neo pixels to solid blue\r\n");
        neopixel.displayBlue();
        break;
    case 2:
        pc.printf("Setting neo pixels to flashing green\r\n");
        neopixel.flashGreen(10,2);
        break;
    default:
        pc.printf("Neo pixels tried to be set to unknow mode\r\n");
        break;
    }
}

void processCANMsg(CANMsg *p_newMsg) {
    // PRINT_INFO("Recieved CAN message with ID %X\r\n", p_newMsg->id);
    // The specific can ID for changing the color of the neopixels is 0x794
    const unsigned int setNeoPixelMode = 0x794;
    switch (p_newMsg->id){
        //case setNeoPixelColorRed:
        // Store 0x794 into vars that are stored in rover_config.h
        case setNeoPixelMode:
            pc.printf("Updating neo pixels\r\n");
            handleSetNeoPixelColor(p_newMsg);
            break;
        default:
            pc.printf("Recieved unimplemented command\r\n");
            break;
    }
}

// main() runs in its own thread in the OS
int main()
{
    initCAN();
    while(1){
        if (can.read(rxMsg)){
            processCANMsg(&rxMsg);
            rxMsg.clear();
        }
    }
}