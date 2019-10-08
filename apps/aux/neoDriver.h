#ifndef NEO_DRIVER_H
#define NEO_DRIVER_H

#ifndef NEO_PIN_NAMES
#define NEO_PIN_NAMES
	enum{				
		NEO_MOSI = PA_6,
		NEO_MISO = PA_7,
		NEO_SCLK = PA_5,
		NEO_SSEL = PA_4
	};
#endif

#include "mbed.h"
#include "SPI.h"

class NeoDriver{

public:	
	//Initializes the NeoPixel driver with the predefined pin names
	NeoDriver();
	
	//Displays the individual bytes to the NeoPixel board
	//data: the RGBW values of the pixels, aranged such that:
	//data[4i] is the R value for the ith pixel
	//data[4i + 1] is the G value for the ith pixel
	//data[4i + 2] is the B value for the ith pixel
	//data[4i + 3] is the W value for the ith pixel
	int displayNP(unsigned char* data, int data_length);
	
	
private:
	SPI NeoPixelSPI;
};

#endif
