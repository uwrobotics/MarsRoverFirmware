/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */


/* TODO: create test driver */
DigitalOut led1(LED1);

// main() runs in its own thread in the OS
int main() {
  while (true) {
    // Blink LED and wait 0.5 seconds
    led1 = !led1;
    ThisThread::sleep_for(500ms);
  }
}
