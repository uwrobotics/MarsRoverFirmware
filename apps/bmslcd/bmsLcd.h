/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BMS_LCD_H
#define BMS_LCD_H

#include "mbed.h"
#include "TextLCD.h"

class bmsLcd
{
    public:
    void bmsLcd::writeToPin(const char* stringToWrite);

    private:
    TextLCD lcd;
};



#endif