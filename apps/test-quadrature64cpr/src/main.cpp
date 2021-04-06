#include "Logger.h"
#include "Quadrature64CPR.h"

// first pin is yellow wire
// second pin is white wire
constexpr Encoder::Quadrature64CPR::Config config = {PA_5, PA_6, NC, 0};
Encoder::Quadrature64CPR encoder(config);

// Synchronous testing
void Updater();
void Reader();
Timer timer;

constexpr auto PERIOD            = 500ms;
constexpr bool USE_BLOCKING_TEST = true;

// Print helper function
void print(const std::string &str);
Mutex print_mutex;

int main() {
  Thread updater_thread(osPriorityNormal), reader_thread(osPriorityNormal);

  updater_thread.start(&Updater);
  reader_thread.start(&Reader);

  updater_thread.join();
  reader_thread.join();
}

void Updater() {
  while (true) {
    if constexpr (USE_BLOCKING_TEST) {
      timer.reset();
      timer.start();
      MBED_ASSERT(encoder.update());
      timer.stop();
      std::string str = "Time taken to update encoder: " + std::to_string(timer.elapsed_time().count()) + "us\r\n";
      print(str);
    } else {
      // not sure what this line of code does, will leave it commmented for now
      // MBED_ASSERT(encoder.update(nullptr));
    }
    ThisThread::sleep_for(PERIOD);
  }
}

void Reader() {
  while (true) {
    std::string str = "Angle: " + std::to_string(encoder.getAngleDeg()) +
                      ", Angular Velocity: " + std::to_string(encoder.getAngularVelocityDegPerSec()) + "\r\n";
    print(str);
    ThisThread::sleep_for(PERIOD);
  }
}

void print(const std::string &str) {
  std::unique_lock<Mutex> lock(print_mutex);
  printf("%s", str.c_str());
}
