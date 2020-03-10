#include "mbed.h"
#include "FSR.h"

FSR force_sensor(PinName::FSR_OUT);

DigitalOut led(PinName::LED1);


int main() {
	while(1) {
		if(force_sensor.getValue() < 0.1 && led){
			led = 0;
		}
		if(force_sensor.getValue() > 0.1 && !led){
			led = 1;
		}
	}
}