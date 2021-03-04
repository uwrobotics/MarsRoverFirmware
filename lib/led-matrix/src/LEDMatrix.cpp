#include "LEDMatrix.h"

#include "mbed.h"

LEDMatrix::LEDMatrix(PinName R, PinName G, PinName B)
    : m_RChannel(R), m_GChannel(G), m_BChannel(B), lightsThread(nullptr), reqEndFlash(0, 1) {}

LEDMatrix::~LEDMatrix() {
  if (lightsThread != nullptr) terminateFlashing();
}

void LEDMatrix::terminateFlashing() {
  reqEndFlash.release();
  lightsThread->join();
  lightsThread->terminate();
  delete lightsThread;
  lightsThread = nullptr;
}
void LEDMatrix::flashing() {
  while (!reqEndFlash.try_acquire()) {
    setColor(flashing_red, flashing_green, flashing_blue);
    ThisThread::sleep_for(PERIOD_DELAY);
    clearLights();
    ThisThread::sleep_for(PERIOD_DELAY);
  }
}

void LEDMatrix::setColor(uint8_t R, uint8_t G, uint8_t B) {
  if (lightsThread != nullptr) {
    terminateFlashing();
  }

  m_RChannel.pulsewidth(R / 255.0);
  m_GChannel.pulsewidth(G / 255.0);
  m_BChannel.pulsewidth(B / 255.0);
}

void LEDMatrix::setColor(HWBRIDGE::LEDMATRIX::color c) {
  if (lightsThread != nullptr) {
    terminateFlashing();
  }

  switch (c) {
    case HWBRIDGE::LEDMATRIX::color::RED:
      setColor(255, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::GREEN:
      setColor(0, 255, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::BLUE:
      setColor(0, 0, 255);
      break;
  }
}

void LEDMatrix::flashColor(uint8_t R, uint8_t G, uint8_t B) {
  flashing_red   = R;
  flashing_green = G;
  flashing_blue  = B;

  if (lightsThread == nullptr) {
    lightsThread = new Thread;
    lightsThread->start(callback(this, &LEDMatrix::flashing));
  }
}

void LEDMatrix::flashColor(HWBRIDGE::LEDMATRIX::color c) {
  flashing_color = c;

  switch (c) {
    case HWBRIDGE::LEDMATRIX::color::RED:
      flashColor(255, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::GREEN:
      flashColor(0, 255, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::BLUE:
      flashColor(0, 0, 255);
      break;
  }
}

void LEDMatrix::clearLights() {
  setColor(0, 0, 0);
}
