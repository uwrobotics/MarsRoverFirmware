#include "AEAT6012.h"
#include "Logger.h"

Encoder::AEAT6012 encoder({SPI_SCK, SPI_MISO, SPI_CS, 0});

Mutex print_mutex;

void print(const std::string &str);  // thread safe print

constexpr auto PERIOD            = 100ms;
constexpr bool USE_BLOCKING_TEST = true;

/* Synchronous testing */
void Updater();
void Reader();

/* Async testing */
void callback();

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
      Timer timer;
      timer.start();
      MBED_ASSERT(encoder.update());
      timer.stop();
      std::string str = "Time taken to update encoder: " + std::to_string(timer.elapsed_time().count()) + "us\r\n";
      print(str);
    } else {
      MBED_ASSERT(encoder.update(nullptr));  // doesnt make sense to time this.
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
