#include "Neopixel_Blocking.h"
#include "hw_bridge.h"
#include "mbed.h"

int main() {
  Neopixel_Blocking pixels(16, LED_MTRX);

  while (1) {
    pixels.displayRed();
    wait(1);
    pixels.displayBlue();
    wait(1);
    pixels.blinkPixels(2, 1, pixels.Green);
  }

  return 1;
}
