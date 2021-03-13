#include "LEDMatrix.h"
#include "mbed.h"

LEDMatrix::LEDMatrix(PinName R, PinName G, PinName B)
    : m_RChannel(R), m_GChannel(G), m_BChannel(B), lightsThread(nullptr) {}

LEDMatrix::~LEDMatrix() {
  if (lightsThread != nullptr) 
  {
    lightsThread->terminate();  //kills thread - check proper termination of thread (lightsThread->join()?)
    delete lightsThread;        //double check this
    lightsThread = nullptr;
  }
  clearLights();
}

void LEDMatrix::flashing() {
  while(true){
    event_flags.wait_all(START_FLASH); //would yield until flag is set
    continue_flashing = true;
    
    while (continue_flashing)
    {
      setColor(flashing_red, flashing_green, flashing_blue);
      ThisThread::sleep_for(PERIOD_DELAY);
      clearLights();
      ThisThread::sleep_for(PERIOD_DELAY);
    }
    event_flags.set(ENDED_FLASH);
  }
}

void LEDMatrix::setFlashColor(bool R, bool G, bool B) {
  flashing_red   = R;
  flashing_green = G;
  flashing_blue  = B;

  if(!continue_flashing){
    if (lightsThread == nullptr) {
      lightsThread = new Thread;
      if (lightsThread == nullptr) 
        return; //raise an error instead of returning
      lightsThread->start(callback(this, &LEDMatrix::flashing)); //why this
    }
    event_flags.set(START_FLASH);
  }
}

void LEDMatrix::setSolidColor(bool R, bool G, bool B){
  if (continue_flashing){  
    continue_flashing = false;
    event_flags.wait_all(ENDED_FLASH);
  }
  setColor(R, G, B);
}

void LEDMatrix::setColor(bool R, bool G, bool B) {
  m_RChannel = R;
  m_GChannel = G;
  m_BChannel = B;
}

/*
void LEDMatrix::setColor(HWBRIDGE::LEDMATRIX::color c) {

  switch (c) {
    case HWBRIDGE::LEDMATRIX::color::RED:
      setColor(1, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::GREEN:
      setColor(0, 1, 0);
      break;
    case HWBRIDGE::LEDMATRIX::color::BLUE:
      setColor(0, 0, 1);
      break;
  }
}*/


void LEDMatrix::clearLights() {
  setColor(0, 0, 0);
}
