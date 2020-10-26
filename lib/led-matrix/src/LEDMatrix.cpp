/* LED Matrix Source File */

#include "LEDMatrix.h"

// Constructor: An LED Matrix is defined by the pins it is connected to.
LEDMatrix ::LEDMatrix(PinName R, PinName G, PinName B) : _RChannel(R), _GChannel(G), _BChannel(B) {}

// Take in values from 0 to 255 for each color stream and maps them to a PWM signal.
LEDMatrix ::void setColorRGB(uint8_t R, uint8_t G, uint8_t B) {
  _RChannel.pulsewidth((1.0/255.0)*R);
  _GChannel.pulsewidth((1.0/255.0)*G);
  _BChannel.pulsewidth((1.0/255.0)*B);
}

// Just for convenience. Call setColorRGB().
LEDMatrix ::void setColor(color c) {
  switch(c) {     
    case color::RED:
      setColorRGB(255, 0, 0);
      break;
    case color::GREEN:
      setColorRGB(0, 255, 0);
      break;
    case color::BLUE:
      setColorRGB(0, 0, 255);
      break;
  }
}

// Set the specified color, wait, turn off all LEDs, wait, and repeat until another function is called by the main.cpp.
LEDMatrix ::void setFlashingColorRGB(uint8_t R, uint8_t G, uint8_t B, uint8_t frequency_hz) {
  float flash_num = frequency_hz*HW_BRIDGE::LEDMATRIX::NUM_FLASHES;         // Flash for 10sec
  float timeout_ms = (1.0/frequency_hz)*1000;
  
  for (uint8_t i=0; i<flash_num; i++) {
    setColour(R, G, B);
    std::ThisThread::sleep_for(std::chrono::milliseconds(timeout_ms/2));  // Div by 2 cuz theres 2 timeouts in each loop
    clear();
    std::ThisThread::sleep_for(std::chrono::milliseconds(timeout_ms/2));
  }
}

// Just for convenience. Call setFlashingColorRBG().
LEDMatrix ::void setFlashingColor(color c, uint8_t frequency_hz) {
  switch(c) {
    case color::RED:
      setFlashingColorRGB(255, 0, 0, frequency_hz);
      break;
    case color::GREEN:
      setFlashingColorRGB(0, 255, 0, frequency_hz);
      break;
    case color::BLUE:
      setFlashingColorRGB(0, 0, 255, frequency_hz);
      break;
  }    
}

// Turn off all the LEDs. Call setColor(0,0,0).
LEDMatrix ::void clear() {
  setColour(0, 0, 0);
}
