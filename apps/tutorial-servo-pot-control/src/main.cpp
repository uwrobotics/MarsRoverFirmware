#include "mbed.h"

AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);

int main() {

	servoPwmOut.period_ms(20);
	while(1) {
		float potVoltage = potVoltageIn.read();
		servoPwmOut.pulsewidth_ms(1 + potVoltage/3.3);
	}
}
