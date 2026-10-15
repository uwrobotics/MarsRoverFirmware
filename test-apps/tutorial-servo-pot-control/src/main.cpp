#include "mbed.h"

// Hardware objects
AnalogIn potVoltageIn(PA_0);  // potentiometer wiper
PwmOut servoPwmOut(PA_1);     // servo signal line

// Servo timing constants
const int k_periodMs     = 20;    // 50 Hz
const float k_minPulseMs = 1.0f;  // minimum angle
const float k_maxPulseMs = 2.0f;  // maximum angle

int main() {
  servoPwmOut.period_ms(k_periodMs);

  while (true) {
    float potFraction = potVoltageIn.read(); // get new voltage reading every loop
    float pulseMs     = k_minPulseMs + potFraction * (k_maxPulseMs - k_minPulseMs);
    servoPwmOut.pulsewidth(pulseMs / 1000.0f);
    ThisThread::sleep_for(20ms); //wait for 20ms
  }
}
