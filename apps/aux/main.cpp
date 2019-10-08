#define DEVICE_SPI_ASYNCH 1
#include "mbed.h"
#include "ledDriver.h"


int main(){
	
	ledDriver ld = ledDriver();

	ld.flashDisplay(BLUE);
	while(1){
		ld.refreshCurrentDisplay();
	}

	return 0;
}
