#include "LEDMatrix.h"
#include "hw_bridge.h"

Thread *setFlashingColorThread;
Semaphore requestEndFlash;
LEDMatrix blinky(PA_0, PA_1, PB_0, setFlashingColorThread, requestEndFlash);

int main() {
  printf("####### STARTED ########\r\n");
  
  while(true)
  {
    blinky.flashForever(HWBRIDGE::LEDMATRIX::color::RED, 2.0);
    ThisThread::sleep_for(5s);
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::RED);
    ThisThread::sleep_for(5s);
    blinky.flashForever(HWBRIDGE::LEDMATRIX::color::BLUE, 2.0);
    ThisThread::sleep_for(5s);
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::BLUE);
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
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::RED);
    ThisThread::sleep_for(1s);
    printf("Colour: GREEN\n");
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::GREEN);
    ThisThread::sleep_for(1s);
    printf("Colour: BLUE\n");
    blinky.setColor(HWBRIDGE::LEDMATRIX::color::BLUE);
    ThisThread::sleep_for(1s);
    printf("Colour: Violet (238, 130, 238) flashing at 1Hz\n");
    blinky.setFlashingColor(238, 130, 238, 1);
    ThisThread::sleep_for(1s);
    printf("Colour: GREEN flashing at 1Hz\n");
    blinky.setFlashingColor(HWBRIDGE::LEDMATRIX::color::GREEN, 1);
    ThisThread::sleep_for(1s);
    printf("Clear\n");
    blinky.clear();
    ThisThread::sleep_for(5s);
  }
  return true;
}
*/
