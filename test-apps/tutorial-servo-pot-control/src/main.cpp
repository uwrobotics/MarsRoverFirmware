#include "mbed.h"

AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);

int main() {
  // set servo digital signal's period to 20ms
  servoPwmOut.period_ms(20);
  while (1) {
    // 0.0 ~ 3.3V
    float potVoltage = potVoltageIn.read();

    // map 0.0 ~ 3.3 V to 1ms ~ 2ms duty cycle of servo's digital signal
    servoPwmOut.pulsewidth((1 + potVoltage / 3.3) / 1000);
  }
}
