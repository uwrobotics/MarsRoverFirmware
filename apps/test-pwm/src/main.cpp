#include "mbed.h"

PwmOut pwmLED(LED1);

const auto k_period     = 1ms;
constexpr auto interval = 50ms;
const float k_step      = 0.02;

int main() {
  pwmLED.period(k_period);

  float duty   = 0.0f;
  int inverter = 1;

  while (true) {
    if (duty > 1.0f) {
      inverter = -1;
    } else if (duty < 0.0f) {
      inverter = +1;
    }

    duty += inverter * k_step;

    pwmLED.write(duty);

    ThisThread::sleep_for(interval);
  }
}
