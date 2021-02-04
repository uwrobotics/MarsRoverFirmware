/* LED Matrix Source File */

//naming with m_
#include "LEDMatrix.h"
#include "mbed.h"

class LEDMatrix {
  private:
    void terminateFlashing() {
      requestEndFlashing.release(); // ask flashing thread to end - signal semaphore
      lightsThread->join(); // wait for the thread to finish. we've asked it to finish so should be quick wait
      lightsThread->terminate();
      delete lightsThread; 
      lightsThread = nullptr;
    }
    void flashing() {
      while(!requestEndFlashing.try_acquire()) { // while we havent been asked to terminate this thread
        printf("Flashing thread\r\n");
        ThisThread::sleep_for(1s);
        // do flashing with flashing_color which is a member var

        constexpr std::chrono::milliseconds period_delay = flash_period_sec * 1000;

        //assume flashing will only be called for R, G, or B
        while (true) {
          setColor(flashing_colour); //is okay that it calls the thread that tries to terminate it... nvmm
          ThisThread::sleep_for(period_delay);
          clearLights();
          ThisThread::sleep_for(period_delay);
        }
      }
    }
  
  
  public:
    // init thread ptr to nullptr and initialize the requestEndFlashing semaphore to 0 and a max of 1
    LEDMatrix ::LEDMatrix() : m_RChannel(R), m_GChannel(G), m_BChannel(B), lightsThread(nullptr), requestEndFlashing(0,1) {}     
    
    void LEDMatrix ::setColor(uint8_t R, uint8_t G, uint8_t B) {
      if (lightsThread == nullptr){ // make sure a thread exists before trying to terminate it
        terminateFlashing();
      }

      m_RChannel.pulsewidth(R / 255.0);
      m_GChannel.pulsewidth(G / 255.0);
      m_BChannel.pulsewidth(B / 255.0);
    }

    // Just for convenience. Call setColorRGB().
    void LEDMatrix ::setColor(HWBRIDGE::LEDMATRIX::color c) {
      if (lightsThread == nullptr){
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

    void flashForever (HWBRIDGE::LEDMATRIX::color c, float period_sec) {
      flashing_color = c; // we have to store the arg like this bc the callback in line33 cant take args (also must return void btw)
      flash_period_sec = period_sec;

      if(lightsThread == nullptr) { // make sure to not create a thread if another one is running
        lightsThread = new Thread;
        lightsThread->start(callback(this, &LEDMatrix::flashing)); 
      }
    }

    void LEDMatrix ::clearLights() {
      setColor(0, 0, 0);
    }
};

/*
    void flashForeverColor (uint8_t R, uint8_t G, uint8_t B, float period) {
      setR = R;
      setB = B;
      setG = B;
      flash_period_sec = period;

      if(lightsThread == nullptr) { // make sure to not create a thread if another one is running
        lightsThread = new Thread;
        lightsThread->start(callback(this, &LEDMatrix::flashing)); 
      }
    }*/


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
void LEDMatrix ::setColor(HWBRIDGE::LEDMATRIX::color c) {
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
void LEDMatrix ::setFlashingColor(HWBRIDGE::LEDMATRIX::color c, float period_sec) {
  switch (c) {
    case HWBRIDGE::LEDMATRIX::color::RED:
      setFlashingColor(255, 0, 0, period_sec);
      break;
    case HWBRIDGE::LEDMATRIX::color::GREEN:
      setFlashingColor(0, 255, 0, period_sec);
      break;
    case HWBRIDGE::LEDMATRIX::color::BLUE:
      setFlashingColor(0, 0, 255, period_sec);
      break;
  }
}

// Turn off all the LEDs. Call setColor(0,0,0).
void LEDMatrix ::clear() {
  setColor(0, 0, 0);
}
*/