#include "LEDMatrix.h"
#include "hw_bridge.h"

int main() {
  printf("####### STARTING ########\r\n");

  LEDMatrix blinky(PA_0, PA_1, PB_0);

  while (true) {
    blinky.flashColor(HWBRIDGE::LEDMATRIX::color::RED);
    ThisThread::sleep_for(5s);
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::RED);
    ThisThread::sleep_for(5s);
    blinky.flashColor(HWBRIDGE::LEDMATRIX::color::GREEN);
    ThisThread::sleep_for(5s);
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::GREEN);
    ThisThread::sleep_for(5s);
    blinky.flashColor(HWBRIDGE::LEDMATRIX::color::BLUE);
    ThisThread::sleep_for(5s);
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::BLUE);
    blinky.clearLights();
  }
}
