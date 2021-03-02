/* LED Matrix Header */
#pragma once
#include "hw_bridge.h"
class LEDMatrix {
 private:
  DigitalOut m_RChannel;
  DigitalOut m_GChannel;  
  DigitalOut m_BChannel;

  //HWBRIDGE::LEDMATRIX::Color flashing_color;
  uint8_t flashing_red, flashing_green, flashing_blue;

  //float flash_period_sec;
  Thread *lightsThread;
  Semaphore reqEndFlash;
  static constexpr auto period_delay = 500ms; //std chrono literal
  void terminateFlashing();
  void flashing();

 public:
  // Constructor: An LED Matrix is defined by the pins it is connected to.
  LEDMatrix(PinName R, PinName G, PinName B);

  // Take in values from 0 to 255 for each pin and map them to a PWM signal.
  void setColor(uint8_t mode);

  // Just for convenience. Call setColorRGB().
  //void setColor(HWBRIDGE::LEDMATRIX::Color c);

  // Set the specified color, wait, turn off all LEDs, wait
  // Repeat until another function is called by main.cpp.
  void flashGreen();

  //void flashColor(HWBRIDGE::LEDMATRIX::Color c);

  // void setFlashingColor(uint8_t R, uint8_t G, uint8_t B, float period_sec);

  // Just for convenience. Call setFlashingColorRGB().
  // void setFlashingColor(HWBRIDGE::LEDMATRIX::Color c, float period_sec);

  // Turn off all the LEDs. Call setColor(0,0,0).
  void clearLights();
};
