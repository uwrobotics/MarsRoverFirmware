/* LED Matrix Header */
#pragma once
#include <mbed.h>
#include <CANMsg.h>
class LEDMatrix
{
  public:
	// Constructor: An LED Matrix is defined by the pins it is connected to
	LEDMatrix(PinName R, PinName G, PinName B);
	
	// This function takes in values from 0 to 255 for each color stream and maps them to a PWM signal
	void setColor(uint8_t R, uint8_t G, uint8_t B);
	
	// This function is just for convenience. It simply calls the above function. For example if RED is passed in, call the above function with setColor(255, 0, 0)
	void setColor(color c);
	
	// This function should continously set the specified color, wait some time, turn off all LEDs, wait some time and repeat until another function is called by the main.cpp
	void setFlashingColor(uint8_t R, uint8_t G, uint8_t B, uint8_t frequency_hz);
	
	// This function is just for convenience. It simply calls the above function. For example if RED is passed in, call the above function with setColor(255, 0, 0)
	void setFlashingColor(color c, uint8_t frequency_hz);
	// This function should turn off all the LEDs. This should just call setColor(0,0,0) - zero pulsewidth
	
	void clear();
	// This is just for convenience
	
	enum class color 
	{
		RED, GREEN, BLUE
	};
	
  private:
	PwmOut _RChannel;
	PwmOut _GChannel;
	PwmOut _BChannel;
	//any other functions 
};
