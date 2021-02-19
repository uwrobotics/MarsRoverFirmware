/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Logger.h"
#include "mbed.h"

// Blinking rate in millisecond
constexpr auto BLINKING_RATE = 500ms;

int main() {
  // Initialise the digital pin LED1 as an output
  DigitalOut led(LED1);

  while (true) {
    led = !led;
    Utility::Logger::printf("Current LED State is %s\n", led ? "ON" : "OFF");
    ThisThread::sleep_for(BLINKING_RATE);
  }
}