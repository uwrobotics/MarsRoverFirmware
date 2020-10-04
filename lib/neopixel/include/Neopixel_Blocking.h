#pragma once
#include "mbed.h"

/*
    Data is written in G -> R -> B
    Each byte is a number value from 0-255 of the intensity of that individual LED light
*/
class Neopixel_Blocking {
 private:
  int m_pixelNum;
  int m_on_buffer[8]  = {1, 1, 1, 1, 1, 1, 1, 1};
  int m_off_buffer[8] = {0};
  DigitalOut out;

 public:
  enum colour { Green, Red, Blue, White, Off };
  Neopixel_Blocking(int numPixels, PinName mtrxPinName);
  ~Neopixel_Blocking();
  void pulse_1();
  void pulse_0();
  void writeByte(const int buffer[8]);
  void showColour(colour selectedColour);
  void blinkPixels(int numflashes, std::chrono::duration<float> delay, colour selectedColour);
  void writeAnyRGB(const int colour[3]);

  // basic functions for autonomy
  void displayRed();
  void displayBlue();
  void flashGreen(int numFlashes, std::chrono::duration<float> delay);
  void shutdown();
};
