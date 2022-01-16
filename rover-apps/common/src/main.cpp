#include "AppConfig.h"
#include "Logger.h"
#include "mbed.h"

Thread periodic_10s_thread(osPriorityNormal1);
Thread periodic_1s_thread(osPriorityNormal2);
Thread periodic_100ms_thread(osPriorityNormal3);
Thread periodic_10ms_thread(osPriorityNormal4);
Thread periodic_1ms_thread(osPriorityNormal5);

void periodic_10s(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : gModules) {
    module->periodic_10s();
  }

  auto nextStartTime = startTime + 10s;
  if (Kernel::Clock::now() > nextStartTime) {
    Utility::logger << "Reseting periodic 10s task timing as it failed to hit the deadline!\n";
    nextStartTime = Kernel::Clock::now() + 10s;
  }
  ThisThread::sleep_until(nextStartTime);
}

void periodic_1s(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : gModules) {
    module->periodic_1s();
  }

  auto nextStartTime = startTime + 1s;
  if (Kernel::Clock::now() > nextStartTime) {
    Utility::logger << "Reseting periodic 1s task timing as it failed to hit the deadline!\n";
    nextStartTime = Kernel::Clock::now() + 1s;
  }
  ThisThread::sleep_until(nextStartTime);
}

void periodic_100ms(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : gModules) {
    module->periodic_100ms();
  }

  auto nextStartTime = startTime + 100ms;
  if (Kernel::Clock::now() > nextStartTime) {
    Utility::logger << "Reseting periodic 100ms task timing as it failed to hit the deadline!\n";
    nextStartTime = Kernel::Clock::now() + 100ms;
  }
  ThisThread::sleep_until(nextStartTime);
}

void periodic_10ms(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : gModules) {
    module->periodic_10ms();
  }

  auto nextStartTime = startTime + 10ms;
  if (Kernel::Clock::now() > nextStartTime) {
    Utility::logger << "Reseting periodic 10ms task timing as it failed to hit the deadline!\n";
    nextStartTime = Kernel::Clock::now() + 10ms;
  }
  ThisThread::sleep_until(nextStartTime);
}

void periodic_1ms(void) {
  auto startTime = Kernel::Clock::now();
  for (Module* module : gModules) {
    module->periodic_1ms();
  }

  auto nextStartTime = startTime + 1ms;
  if (Kernel::Clock::now() > nextStartTime) {
    Utility::logger << "Reseting periodic 1ms task timing as it failed to hit the deadline!\n";
    nextStartTime = Kernel::Clock::now() + 1ms;
  }
  ThisThread::sleep_until(nextStartTime);
}

int main() {
  periodic_1ms_thread.start(periodic_1ms);
  periodic_10ms_thread.start(periodic_10ms);
  periodic_100ms_thread.start(periodic_100ms);
  periodic_1s_thread.start(periodic_1s);
  periodic_10s_thread.start(periodic_10s);

  while (true)
    ;
}
