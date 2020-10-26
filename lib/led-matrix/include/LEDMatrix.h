/* LED Matrix Header */
#pragma once
#include <CANMsg.h>
#include <mbed.h>
class LEDMatrix {
  public:
	// Constructor: An LED Matrix is defined by the pins it is connected to.
	LEDMatrix(PinName R, PinName G, PinName B);
	
	// Take in values from 0 to 255 for each color stream and maps them to a PWM signal.
	void setColorRGB(uint8_t R, uint8_t G, uint8_t B);
	
	// Just for convenience. Call setColorRGB().
	void setColor(color c);
	
	// Set the specified color, wait, turn off all LEDs, wait, and repeat until another function is called by the main.cpp.
	void setFlashingColorRGB(uint8_t R, uint8_t G, uint8_t B, uint8_t frequency_hz);
	
	// Just for convenience. Call setFlashingColorRGB().
	void setFlashingColor(color c, uint8_t frequency_hz);

	// This function should turn off all the LEDs. This should just call setColor(0,0,0) - zero pulsewidth
	void clear();
	// This is just for convenience
	
	enum class color { RED, GREEN, BLUE};
	
  private:
    PwmOut _RChannel;
	PwmOut _GChannel;
	PwmOut _BChannel;
	//any other functions 
};
