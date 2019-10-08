#include "ledDriver.h"


	ledDriver::ledDriver(): nDriver(){
		pixNo = 0;
		ddL = PIXELWIDTH * PIXELHEIGHT * 4;
		displayType = 2;
		colour = RED;
		currentCharacter = 'A';
		prevColour = 0;
		loopsSinceFlash = 0;
		
		
		pixNo = PIXELWIDTH * PIXELHEIGHT;
	}
	
	//Displays a charachter to the matrix, only works with 8x8
	mbed_error_status_t ledDriver::displayChar(char ch, int col){
		
		if(pixNo != 64){
			return MBED_SUCCESS;
		}
		
		long long charmap = font[ch];
		int tempCol = col;
		for(int i = 0;i < 8;i++){
			for(int k = 0;k < 8;k++){
				if(charmap%2 == 1){
					displayData[i*32 + k*4 + 3] = tempCol%0xFF;
					tempCol = tempCol>>8;
					displayData[i*32 + k*4 + 2] = tempCol%0xFF;
					tempCol = tempCol>>8;
					displayData[i*32 + k*4 + 1] = tempCol%0xFF;
					tempCol = tempCol>>8;
					displayData[i*32 + k*4 + 0] = tempCol%0xFF;
					tempCol = col;
				}else{
					displayData[i*32 + k*4 + 3] = 0;
					displayData[i*32 + k*4 + 2] = 0;
					displayData[i*32 + k*4 + 1] = 0;
					displayData[i*32 + k*4 + 0] = 0;
				}
				charmap = charmap << 1;
			}
		}
		displayType = 0;
		colour = col;
		currentCharacter = ch;
		
		if(nDriver.displayNP(displayData, ddL) == 0){
			return MBED_SUCCESS;
		}
		return MBED_MAKE_CUSTOM_ERROR(MBED_MODULE_DRIVER, SPI_ERROR);
	}
	
	//Flashes the display rapidly
	mbed_error_status_t ledDriver::flashDisplay(int col){
		int temp = col;
		for(int i = 0;i < pixNo;i++){
			displayData[4 * i + 3] = temp%0xFF;
			temp = temp>>8;
			displayData[4 * i + 2] = temp%0xFF;
			temp = temp>>8;
			displayData[4 * i + 1] = temp%0xFF;
			temp = temp>>8;
			displayData[4 * i + 0] = temp%0xFF;
			temp = col;
		}
		displayType = 1;
		colour = col;
		if(nDriver.displayNP(displayData, ddL) == 0){
			return MBED_SUCCESS;
		}
		return MBED_MAKE_CUSTOM_ERROR(MBED_MODULE_DRIVER, SPI_ERROR);
	}
	
	//Sets the screen to be one solid colour
	mbed_error_status_t ledDriver::solidColour(int col){
		int temp = col;
		for(int i = 0;i < pixNo;i++){
			displayData[4 * i + 3] = temp%0xFF;
			temp = temp<<8;
			displayData[4 * i + 2] = temp%0xFF;
			temp = temp<<8;
			displayData[4 * i + 1] = temp%0xFF;
			temp = temp<<8;
			displayData[4 * i + 0] = temp%0xFF;
			temp = col;
		}
		displayType = 2;
		colour = col;
		if(nDriver.displayNP(displayData, ddL) == 0){
			return MBED_SUCCESS;
		}
		return MBED_MAKE_CUSTOM_ERROR(MBED_MODULE_DRIVER, SPI_ERROR);
	}
	
	
	mbed_error_status_t ledDriver::refreshCurrentDisplay(){
		mbed_error_status_t tr = MBED_SUCCESS;
		switch(displayType){
			case 1:
			if(loopsSinceFlash > flashLength){
				loopsSinceFlash = 0;
				if(colour == 0){
					tr = flashDisplay(prevColour);
				}else{
					prevColour = colour;
					tr = flashDisplay(0);
				}
			}else{
				loopsSinceFlash++;
			}
			break;
			case 0:
			case 2:	
				if(nDriver.displayNP(displayData, ddL) == 0){
					tr = MBED_SUCCESS;
				}else{
					tr = MBED_MAKE_CUSTOM_ERROR(MBED_MODULE_DRIVER, SPI_ERROR);
				}
			break;
		}
		return tr;
	}	
