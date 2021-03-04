#pragma once
#include "hw_bridge.h"
class LEDMatrix {
 private:
  PwmOut m_RChannel;
  PwmOut m_GChannel;
  PwmOut m_BChannel;

  HWBRIDGE::LEDMATRIX::color flashing_color;
  volatile uint8_t flashing_red, flashing_green, flashing_blue;

  Thread *lightsThread;
  Semaphore reqEndFlash;
  static constexpr auto PERIOD_DELAY = 500ms;
  void terminateFlashing();
  void flashing();

 public:
  // Define matrix by the pins it is connected to.
  LEDMatrix(PinName R, PinName G, PinName B);

  // Delete the pointer to the thread if it has not been deleted already.
  ~LEDMatrix();

  // Take in values from 0 to 255 for each pin and map them to a PWM signal.
  void setColor(uint8_t R, uint8_t G, uint8_t B);

  // Call setColor with the enum color.
  void setColor(HWBRIDGE::LEDMATRIX::color c);

  // Set the specified color, wait, turn off all LEDs, wait
  // Repeat until another function is called by main.cpp.
  void flashColor(uint8_t R, uint8_t G, uint8_t B);

  // Call falshColor with the enum color.
  void flashColor(HWBRIDGE::LEDMATRIX::color c);

  // Turn off all the LEDs. Call setColor(0,0,0).
  void clearLights();
};
