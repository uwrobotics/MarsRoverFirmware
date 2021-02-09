/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ActuatorController.h"

/* TODO: create test driver */

// main() runs in its own thread in the OS
int main() {
  while (true) {
    // Blink LED and wait 0.5 seconds
    led1 = !led1;
    ThisThread::sleep_for(500ms);
  }
}
