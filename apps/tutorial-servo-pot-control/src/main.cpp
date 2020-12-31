/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"  //header file for microcontroller library

// initializing objects from mbed.h
AnalogIn potVoltageIn(
    PA_0);  // initializing pin 0 to read voltage drop across potentiometer (thus, reading the position of the servo)
PwmOut servoPwmOut(
    PA_1);  // initializing pin 1 to output the PWM signal generated based on the potentiometer's read position
 
int main() {
  servoPwmOut.period(0.020);  // set PWM period to 20ms (50 Hz). Pulse times vary within the 20ms period
 
  while (true)  // go forever
  {
// initialize the voltage to a variable
    float potVoltage = potVoltageIn.read();
// voltage ranges from 0.0V to 3.3V. Map to duty cycle range of min 1ms and max 2ms & output
    servoPwmOut.pulsewidth(0.001 + 0.001 * potVoltage / 3.3);  
  }
}