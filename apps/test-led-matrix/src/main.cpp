#include "LEDMatrix.h"
#include "hw_bridge.h"

int main() {
  printf("####### STARTING ########\r\n");

  LEDMatrix blinky(PA_0, PA_1, PB_0);

  while (true) {
    blinky.setFlashColor(1, 0, 0);
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(1, 0, 0);
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(0, 1, 0);
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(0, 1, 0);
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(0, 0, 1);
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(0, 0, 1);
    blinky.clearLights();
  }
}
