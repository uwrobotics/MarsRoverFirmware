#define DEVICE_SPI_ASYNCH 1
#include "../../targets/nucleo/PinNames.h"
#include "mbed.h"
#include "ledDriver.h"

Serial pc(SERIAL_TX, SERIAL_RX, 115200);

int main(){
	
	pc.printf("Starting up\n");

	ledDriver ld = ledDriver();

	ld.solidColour(RED);
	while(1){
		ld.refreshCurrentDisplay();
		wait(0.5);
	}

	return 0;
}
