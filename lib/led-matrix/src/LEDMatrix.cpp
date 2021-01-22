/* LED Matrix Source File */

#include "LEDMatrix.h"

// Constructor: An LED Matrix is defined by the pins it is connected to.
LEDMatrix ::LEDMatrix(PinName R, PinName G, PinName B) : m_RChannel(R), m_GChannel(G), m_BChannel(B) {}

// Take in values from 0 to 255 for each pin and map them to a PWM signal.
void LEDMatrix ::setColor(uint8_t R, uint8_t G, uint8_t B) {
  m_RChannel.pulsewidth(R / 255.0);
  m_GChannel.pulsewidth(G / 255.0);
  m_BChannel.pulsewidth(B / 255.0);
}

// Just for convenience. Call setColorRGB().
void LEDMatrix ::setColor(HWBRIDGE::LEDMATRIX::color c) {
  switch (c) {
    case HWBRIDGE::LEDMATRIX::color::RED:
      setColorRGB(255, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::GREEN:
      setColorRGB(0, 255, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::BLUE:
      setColorRGB(0, 0, 255);
      break;
  }
}

// Set the specified color, wait, turn off all LEDs, wait
// Repeat until another function is called by main.cpp.
// NEED THREAD
void LEDMatrix ::setFlashingColor(uint8_t R, uint8_t G, uint8_t B, float period_sec) {
  // float flash_num     = frequency_hz * HWBRIDGE::LEDMATRIX::NUM_FLASHES;  // Flash for 10sec
  // uint16_t timeout_ms = (1.0 / frequency_hz) * 1000;
  // How long should each flash last?

  constexpr std::chrono::milliseconds period_delay = period_sec * 1000;

  while (true) {
    setColorRGB(R, G, B);
    ThisThread::sleep_for(period_delay);
    clear();
    ThisThread::sleep_for(period_delay);
  }
}

// Just for convenience. Call setFlashingColorRBG().
void LEDMatrix ::setFlashingColor(HWBRIDGE::LEDMATRIX::color c, float period_sec) {
  switch (c) {
    case HWBRIDGE::LEDMATRIX::color::RED:
      setFlashingColorRGB(255, 0, 0, period_sec);
      break;
    case HWBRIDGE::LEDMATRIX::color::GREEN:
      setFlashingColorRGB(0, 255, 0, period_sec);
      break;
    case HWBRIDGE::LEDMATRIX::color::BLUE:
      setFlashingColorRGB(0, 0, 255, period_sec);
      break;
  }
}

// Turn off all the LEDs. Call setColor(0,0,0).
void LEDMatrix ::clear() {
  setColorRGB(0, 0, 0);
}
