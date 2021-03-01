#include "LEDMatrix.h"
#include "hw_bridge.h"

//Thread setFlashingColorThread;
//Semaphore requestEndFlash;

int main() {
  printf("####### STARTED ########\r\n");

  LEDMatrix blinky(PA_0, PA_1, PB_0);

  while (true) {
    blinky.flashColor(255,0,0);
    ThisThread::sleep_for(5s);
    blinky.setColor(255,0,0);
    ThisThread::sleep_for(5s);
    blinky.flashColor(0,0,255);
    ThisThread::sleep_for(5s);
    blinky.setColor(0,255,0);
    blinky.clearLights();
  }
}

/*
Thread setFlashingColorThread;

int main() {
  LEDMatrix blinky(PA_0, PA_1, PB_0);

  setFlashingColorThread.start(setFlashingColor);

  while (true) {
    printf("Colour: Violet (238, 130, 238)\n");
    blinky.setColor(238, 130, 238);
    ThisThread::sleep_for(1s);
    printf("Colour: RED\n");
    blinky.setColor(HWBRIDGE::LEDMATRIX::Color::RED);
    ThisThread::sleep_for(1s);
    printf("Colour: GREEN\n");
    blinky.setColor(HWBRIDGE::LEDMATRIX::Color::GREEN);
    ThisThread::sleep_for(1s);
    printf("Colour: BLUE\n");
    blinky.setColor(HWBRIDGE::LEDMATRIX::Color::BLUE);
    ThisThread::sleep_for(1s);
    printf("Colour: Violet (238, 130, 238) flashing at 1Hz\n");
    blinky.setFlashingColor(238, 130, 238, 1);
    ThisThread::sleep_for(1s);
    printf("Colour: GREEN flashing at 1Hz\n");
    blinky.setFlashingColor(HWBRIDGE::LEDMATRIX::Color::GREEN, 1);
    ThisThread::sleep_for(1s);
    printf("Clear\n");
    blinky.clear();
    ThisThread::sleep_for(5s);
  }
  return true;
}
*/
