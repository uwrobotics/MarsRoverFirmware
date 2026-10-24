#include "mbed.h"

AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);

int main() {
  servoPwmOut.period_ms(20);

  while (true) {
    // AnalogIn::read() is normalized to [0.0, 1.0] of VREF, so map it directly onto 1ms - 2ms
    float potFraction = potVoltageIn.read();
    servoPwmOut.pulsewidth((1.0f + potFraction) / 1000.0f);
  }
}
