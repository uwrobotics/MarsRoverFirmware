#include "LEDMatrix.h"
#include "hw_bridge.h"

int main() {
  printf("####### STARTING LED MATRIX TEST #######\r\n");

  LEDMatrix blinky(D8, D9, D10);

  while (true) {
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_RED);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_BLUE);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_GREEN);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_RED);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_BLUE);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_GREEN);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF);
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(1, 0, 0);  // red
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(0, 1, 0);  // green
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(0, 0, 1);  // blue
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF);
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(1, 0, 0);  // red
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(0, 1, 0);  // green
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(0, 0, 1);  // blue
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF);
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(1, 0, 1);  // purple
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(1, 1, 0);  // brownish
    ThisThread::sleep_for(5s);
    blinky.setFlashColor(0, 1, 1);  // blue-green
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF);
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(1, 0, 1);  // purple
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(1, 1, 0);  // brownish
    ThisThread::sleep_for(5s);
    blinky.setSolidColor(0, 1, 1);  // blue-green
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_RED);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_GREEN);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_BLUE);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_GREEN);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_BLUE);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_RED);
    ThisThread::sleep_for(5s);
    blinky.setState(HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF);
  }
}
