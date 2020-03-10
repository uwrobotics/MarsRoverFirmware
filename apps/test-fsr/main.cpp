#include "mbed.h"
#include "FSR.h"

FSR force_sensor(FSR_OUT);

DigitalOut led_1(LED1);
DigitalOut led_2(LED2);


int main() {
	led_1 = 0;
	led_2 = 0;
	while(1) {
		if(force_sensor.getValue() > 0.7){
			led_1 = 0;
			led_2 = 0;
		}
		if(force_sensor.getValue() < 0.7){
			led_1 = 1;
			led_2 = 1;
		}
	}
}