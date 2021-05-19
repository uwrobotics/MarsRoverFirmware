#include "Neopixel_Blocking.h"
#include "hw_bridge.h"
#include "mbed.h"

int main() {
  Neopixel_Blocking pixels(16, LED1);

  while (1) {
    pixels.displayRed();
    ThisThread::sleep_for(1ms);
    pixels.displayBlue();
    ThisThread::sleep_for(1ms);
    pixels.blinkPixels(2, 1s, pixels.Green);
  }

  return 1;
}
