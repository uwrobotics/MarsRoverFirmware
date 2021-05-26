#include "mbed.h"

//initilaize potentiometer voltage input on pin PA_0
AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);

int main() {

//loop runs infinitely.
    while(1){
        //sets pulse width modulation period to 20ms(50hz)
        servoPwmOut.period_ms(20);

        float potVoltage = potVoltageIn.read(); 
        servoPwmOut.pulsewidth_ms(1 + potVoltage/3.3);
    }

}