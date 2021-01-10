/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);

int main() {
  servoPwmOut.period_ms(20);
  while (true) {
    float potVoltage = potVoltageIn.read();
    servoPwmOut.pulsewidth((1 + potVoltage / 3.3) / 1000);
  }
}
