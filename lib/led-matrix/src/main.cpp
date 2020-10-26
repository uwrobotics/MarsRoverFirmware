/* LED Matrix Source File */

#include "LEDMatrix.h"

int main() {
  LEDMatrix blinky(PA_1);

  while(true) {
    blinky.setColorRGB(238, 130, 238); //violet
        
    std::ThisThread::sleep_for(std::chrono::milliseconds(1000));
    
    blinky.setColor(RED);
    std::ThisThread::sleep_for(std::chrono::milliseconds(1000));
    blinky.setColor(GREEN);
    std::ThisThread::sleep_for(std::chrono::milliseconds(1000));
    blinky.setColor(BLUE);
    
    std::ThisThread::sleep_for(std::chrono::milliseconds(1000));

    blinky.setFlashingColorRGB(238, 130, 238, 2);
    std::ThisThread::sleep_for(std::chrono::milliseconds(1000));
    blinky.setFlashingColor(GREEN, 2);
    std::ThisThread::sleep_for(std::chrono::milliseconds(1000));
    blinky.clear();
  }
  return true;
}
