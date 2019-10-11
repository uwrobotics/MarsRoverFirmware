#ifndef LED_DRIVER_H
#define LED_DRIVER_H
#define SPI_ERROR 172
#define PIXELWIDTH 10
#define PIXELHEIGHT 1

#include "mbed.h"
#include "neoDriver.h"

enum{
	RED 	= 0x00FF00,
	GREEN 	= 0xFF0000,
	BLUE 	= 0x0000FF,
	WHITE 	= 0xFFFFFF
};


class ledDriver{
	
public:
	//Initializes the led Driver for the matrix
	ledDriver();
	
	//Displays a charachter to the matrix, only works with 8x8
	mbed_error_status_t displayChar(char ch, int col = RED);
	
	//Flashes the display rapidly
	mbed_error_status_t flashDisplay(int col = RED);
	
	//Sets the screen to be one solid colour
	mbed_error_status_t solidColour(int col = RED);
	
	mbed_error_status_t refreshCurrentDisplay();
	

private:
	
	NeoDriver nDriver;
	const static int flashLength = 100;//How long is the flash
	unsigned char displayData[PIXELWIDTH * PIXELHEIGHT * 4];

	//This font is from https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source
	//It has a copyright: Copyright (C) Lisa Milne 2014 <lisa@ltmnet.com>
	//And a license: GNU General Public License version 3
	static long long font[];
	
	int pixNo;//Number of pixels
	int ddL;//display Data Array length
	int displayType;//Display mode (For Refresh)
	int colour;//Display colour (For Refresh)
	char currentCharacter;//Current charachter being displayed
	int prevColour;//For flash
	int loopsSinceFlash;//For Flash

};
#endif
