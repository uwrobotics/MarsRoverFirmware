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
    bmsLcd(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7, LCDType type = TextLCD::LCD16x2);

    void bmsLcd::writeToLcd(const char* firstLine, const char* secondLine)

    private:
    TextLCD lcd;
};



#endif