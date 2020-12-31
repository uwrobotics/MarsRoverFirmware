include "mbed.h" int main() { 
    
    AnalogIn potVoltageIn(PA_0); 
    PwmOut servoPwmOut(PA_1);

    servoPwmOut.period_ms(20); 

    while ( 1 ) { 

        float potVoltage = potVoltageIn.read(); 
        servoPwmOut.pulsewidth_ms(1 + potVoltage/3.3);
    } 

}