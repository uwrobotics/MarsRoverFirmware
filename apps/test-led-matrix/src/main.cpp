#include "LEDMatrix.h"
#include "hw_bridge.h"

int main() {
  printf("####### STARTING #######\r\n");

  LEDMatrix blinky(PA_0, PA_1, PB_0);

  while (true) {
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_RED);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_RED);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_RED);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF);
  }
}
