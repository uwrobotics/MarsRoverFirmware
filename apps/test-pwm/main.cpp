#include "SystemReport.h"
#include "mbed.h"

PwmOut pwmLED(LED1);

const int k_period_ms = 1;
const int interval_ms = 50;
const float k_step = 0.02;

int main() {
  pwmLED.period_ms(k_period_ms);

  float duty = 0.0f;
  int inverter = 1;

  while (true) {
    if (duty > 1.0f) {
      inverter = -1;
    } else if (duty < 0.0f) {
      inverter = +1;
    }

    duty += inverter * k_step;

    pwmLED.write(duty);

    wait_ms(interval_ms);
  }
}
