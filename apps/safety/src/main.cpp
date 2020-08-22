#include "mbed.h"

DigitalOut led1(LED1);

constexpr auto k_interval = 500ms;

int main() {
  while (true) {
    led1 = !led1;
    ThisThread::sleep_for(k_interval);
  }
}
