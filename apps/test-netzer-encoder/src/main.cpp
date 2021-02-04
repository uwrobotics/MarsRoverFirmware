#include "NetzerEncoder.h"
#include "mbed.h"

int main(){
	NetzerEncoder encoder(500000);
	encoder.spi_read();
}
