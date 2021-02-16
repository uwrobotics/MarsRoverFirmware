/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Blinking rate in milliseconds
#define BLINKING_RATE 2000ms

int main() {
  // Initialise the digital pin LED1 as an output
  DigitalOut led(LED1);

  while (true) {
    led = !led;
    printf("Current LED State is %s\n", led ? "ON" : "OFF");
    printf("This is Dhruv \n");
    ThisThread::sleep_for(BLINKING_RATE);
  }
}
