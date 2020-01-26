/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "CANMsg.h"
int main(){
    int a =5;
    a++;
}
// #include "rover_config.h"
// #include "neoPixel.h"

//Serial        pc(SERIAL_TX, SERIAL_RX, 115200);
//CAN           can(PB_8, PB_9, 500000);
//neoPixel      neoPixel_obj(64);
//CANMsg        rxMsg;

// 0x215 CAN ID for change to a neo pixel.
// Color is specified by the data inside the packet
// 0 is solid red
// 1 is solid blue
// 2 is flashing green
/*
void initCAN() {
    // Do not leave as literals, put in rover_config.h
    // CANStandard is defined in CAN.h
    can.filter(0x215, 0xF00, CANStandard);

    // for (int canHandle = firstCommand; canHandle <= lastCommand; canHandle++) {
    //     can.filter(RX_ID + canHandle, 0xFFF, CANStandard, canHandle);
    // }
}

void handleSetNeoPixelColor(CANMsg *p_newMsg){
    // 0 = solid red, 1 = solid blue, 2 = flashing green
    int neoPixelMode = 0;
    *p_newMsg >> neoPixelMode;

    switch (neoPixelMode)
    {
    case 0:
        pc.printf("Setting neo pixels to solid red\r\n");
        //neoPixel_obj.displayRed();
        break;
    case 1:
        pc.printf("Setting neo pixels to sold blue\r\n");
        //neoPixel_obj.displayBlue();
        break;
    case 2:
        pc.printf("Setting neo pixels to flashing green\r\n");
        //neoPixel_obj.displayFlashingGreen();
        break;
    default:
        pc.printf("Neo pixels tried to be set to unknow mode\r\n");
        break;
    }
}

void processCANMsg(CANMsg *p_newMsg) {
    // PRINT_INFO("Recieved CAN message with ID %X\r\n", p_newMsg->id);

    switch (p_newMsg->id) {

        //case setNeoPixelColorRed:
        // Store 0x215 into vars that are stored in rover_config.h
        case 0x215:
            pc.printf("Updating neo pixels\r\n");
            handleSetNeoPixelColor(p_newMsg);
            break;

        default:
            pc.printf("Recieved unimplemented command\r\n");
            break;
    }
}*/

// main() runs in its own thread in the OS
/*
int main()
{
    while(1){
        if (can.read(rxMsg)){
            //processCANMsg(&rxMsg);
        }
    }
    
}*/
