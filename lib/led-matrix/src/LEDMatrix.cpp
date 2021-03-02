/* LED Matrix Source File */

// naming with m_
#include "LEDMatrix.h"

#include "mbed.h"

// init thread ptr to nullptr and initialize the requestEndFlashing semaphore to 0 and a max of 1
LEDMatrix::LEDMatrix(PinName R, PinName G, PinName B)
    : m_RChannel(R), m_GChannel(G), m_BChannel(B), lightsThread(nullptr), reqEndFlash(0, 1) {}

void LEDMatrix::setColor(uint8_t mode) { // this only supports red and blue
  clearLights();
  enum { RED = 0, BLUE = 1 };     // THIS IS WHAT SW SENDS
  if (mode == RED) {
    m_RChannel.write(1);
  } else if (mode == BLUE) {
    m_BChannel.write(1);
  }
}

/* m_RChannel.pulsewidth(R / 255.0);
 m_GChannel.pulsewidth(G / 255.0);
 m_BChannel.pulsewidth(B / 255.0);*/
//}
// Just for convenience. Call setColorRGB().
/*void LEDMatrix::setColor(HWBRIDGE::LEDMATRIX::Color c) {
  if (lightsThread == nullptr) {
    terminateFlashing();
  }

  switch (c) {
    case HWBRIDGE::LEDMATRIX::Color::RED:
      setColor(255, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::Color::GREEN:
      setColor(0, 255, 0);
      break;
    case HWBRIDGE::LEDMATRIX::Color::BLUE:
      setColor(0, 0, 255);
      break;
  }
}*/

void LEDMatrix::flashGreen() {
  clearLights();
  for (uint8_t i = 0; i < 20; i++) {
    m_GChannel.write(1);
    ThisThread::sleep_for(500ms);
    clearLights();
    ThisThread::sleep_for(500ms);
  }
}

/*void LEDMatrix::flashColor(HWBRIDGE::LEDMATRIX::Color c) {
  flashing_color = c;  // we have to store the arg like this bc the callback cant take args (also must return void btw)

  switch (c) {
    case HWBRIDGE::LEDMATRIX::Color::RED:
      flashColor(255, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::Color::GREEN:
      flashColor(0, 255, 0);
      break;
    case HWBRIDGE::LEDMATRIX::Color::BLUE:
      flashColor(0, 0, 255);
      break;
  }
}  */

void LEDMatrix::clearLights() {
  m_RChannel.write(0);
  m_GChannel.write(0);
  m_BChannel.write(0);
}

/*
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
void LEDMatrix ::setColor(HWBRIDGE::LEDMATRIX::Color c) {
  switch (c) {
    case HWBRIDGE::LEDMATRIX::Color::RED:
      setColor(255, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::Color::GREEN:
      setColor(0, 255, 0);
      break;
    case HWBRIDGE::LEDMATRIX::Color::BLUE:
      setColor(0, 0, 255);
      break;
  }
}

// Set the specified color, wait, turn off all LEDs, wait
// Repeat until another function is called by main.cpp.
void LEDMatrix ::setFlashingColor(uint8_t R, uint8_t G, uint8_t B, float period_sec) {

  constexpr std::chrono::milliseconds period_delay = period_sec * 1000;

  while (true) {
    setColor(R, G, B);
    ThisThread::sleep_for(period_delay);
    clear();
    ThisThread::sleep_for(period_delay);
  }
}

// Just for convenience. Call setFlashingColorRBG().
void LEDMatrix ::setFlashingColor(HWBRIDGE::LEDMATRIX::Color c, float period_sec) {
  switch (c) {
    case HWBRIDGE::LEDMATRIX::Color::RED:
      setFlashingColor(255, 0, 0, period_sec);
      break;
    case HWBRIDGE::LEDMATRIX::Color::GREEN:
      setFlashingColor(0, 255, 0, period_sec);
      break;
    case HWBRIDGE::LEDMATRIX::Color::BLUE:
      setFlashingColor(0, 0, 255, period_sec);
      break;
  }
}

// Turn off all the LEDs. Call setColor(0,0,0).
void LEDMatrix ::clear() {
  setColor(0, 0, 0);
}
*/