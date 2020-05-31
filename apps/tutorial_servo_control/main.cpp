/* mbed Microcontroller Library
 */

//#include "SystemReport.h"
#include "mbed.h"

AnalogIn analogInput(PA_0);
PwmOut servoPwm(PA_1);

void init(double servoPwmFrequency)
{
  servoPwm.period(1/servoPwmFrequency);
}

// main() runs in its own thread in the OS
int main() 
{
  init(50) //set nominal servo frequency of 50Hz
  while(true)
  {
    float percentAnalogIn = analogInput.read()/3.3;
    servoPwm.pulsewidth((1 + percentAnalogIn)/1000); //Write the pulse width in seconds
  }
}
