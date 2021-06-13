#include "AppMConfig.h"
#include "mbed.h"

Thread periodic_10s_thread(osPriorityNormal1);
Thread periodic_1s_thread(osPriorityNormal2);
Thread periodic_100ms_thread(osPriorityNormal3);
Thread periodic_10ms_thread(osPriorityNormal4);
Thread periodic_1ms_thread(osPriorityNormal5);

void periodic_10s(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : modules) {
    module->periodic_10s();
  }
  ThisThread::sleep_until(startTime + 10s);
}

void periodic_1s(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : modules) {
    module->periodic_1s();
  }
  ThisThread::sleep_until(startTime + 1s);
}

void periodic_100ms(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : modules) {
    module->periodic_100ms();
  }
  ThisThread::sleep_until(startTime + 100ms);
}

void periodic_10ms(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : modules) {
    module->periodic_10ms();
  }
  ThisThread::sleep_until(startTime + 10ms);
}

void periodic_1ms(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : modules) {
    module->periodic_1ms();
  }
  ThisThread::sleep_until(startTime + 1ms);
}

int main() {
  periodic_1ms_thread.start(periodic_1ms);
  periodic_10ms_thread.start(periodic_10ms);
  periodic_100ms_thread.start(periodic_100ms);
  periodic_1s_thread.start(periodic_1s);
  periodic_10s_thread.start(periodic_10s);

  while (true) {
  }
}
