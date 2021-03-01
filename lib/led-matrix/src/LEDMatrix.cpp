/* LED Matrix Source File */

// naming with m_
#include "LEDMatrix.h"

#include "mbed.h"

// init thread ptr to nullptr and initialize the requestEndFlashing semaphore to 0 and a max of 1
LEDMatrix::LEDMatrix(PinName R, PinName G, PinName B)
    : m_RChannel(R), m_GChannel(G), m_BChannel(B), lightsThread(nullptr), reqEndFlash(0, 1) {}

void LEDMatrix::terminateFlashing() {
  reqEndFlash.release();  // ask flashing thread to end - signal semaphore
  lightsThread->join();   // wait for the thread to finish. we've asked it to finish so should be quick wait
  lightsThread->terminate();
  delete lightsThread;
  lightsThread = nullptr;
}
void LEDMatrix::flashing() {
  while (!reqEndFlash.try_acquire()) {  // while we havent been asked to terminate this thread
    printf("Flashing thread\r\n");
    // do flashing with flashing_color which is a member var

    while (true) {
      setColor(flashing_red, flashing_green,
               flashing_blue);  // is okay that it calls the thread that tries to terminate it... nvmm
      ThisThread::sleep_for(period_delay);
      clearLights();
      ThisThread::sleep_for(period_delay);
    }
  }
}

void LEDMatrix::setColor(uint8_t R, uint8_t G, uint8_t B) {
  if (lightsThread == nullptr) {  // make sure a thread exists before trying to terminate it
    terminateFlashing();
  }

  m_RChannel.pulsewidth(R / 255.0);
  m_GChannel.pulsewidth(G / 255.0);
  m_BChannel.pulsewidth(B / 255.0);
}
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

void LEDMatrix::flashColor(uint8_t R, uint8_t G, uint8_t B) {
  flashing_red   = R;
  flashing_green = G;
  flashing_blue  = B;

  if (lightsThread == nullptr) {  // make sure to not create a thread if another one is running
    lightsThread = new Thread;
    lightsThread->start(callback(this, &LEDMatrix::flashing));
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
  setColor(0, 0, 0);
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