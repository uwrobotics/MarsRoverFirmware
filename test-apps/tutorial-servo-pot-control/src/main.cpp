#include "mbed.h"

AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);

int main() {
  servoPwmOut.period_ms(20);

  while (true) {
    // AnalogIn returns a fraction of the reference voltage in the range 0 to 1.
    const float position = potVoltageIn.read();
    servoPwmOut.pulsewidth((1.0f + position) / 1000.0f);
    ThisThread::sleep_for(20ms);
  }
}
