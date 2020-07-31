/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */


#include "bmsLcd.h"


void bmsLcd::writeToLcd(const char* firstLine, const char* secondLine)
{
    lcd.printf(firstLine);
    lcd.printf("\n")
    lcd.printf(secondLine);

}

bmsLcd::bmsLcd(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7, LCDType type)
{
    lcd(rs, e, d4, d5, d6, d7, type);
    lcd.printf("BMS INIT...");
}