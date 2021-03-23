#include "AEAT6012.h"
#include "Logger.h"

Encoder::AEAT6012 encoder({SPI_SCK, SPI_MISO, SPI_CS, 0});

Mutex mutex;
void print(const std::string &str);  // thread safe print

constexpr auto PERIOD            = 100ms;
constexpr bool USE_BLOCKING_TEST = true;

/* Synchronous testing */
void Updater();
void Reader();

/* Async testing */
void callback();

int main() {
  if constexpr (USE_BLOCKING_TEST) {
    Thread updater_thread(osPriorityNormal), reader_thread(osPriorityNormal);

    updater_thread.start(&Updater);
    reader_thread.start(&Reader);

    updater_thread.join();
    reader_thread.join();
  } else {
    while (true) {
      MBED_ASSERT(encoder.update(callback));
      ThisThread::sleep_for(PERIOD);
    }
  }
}

void Updater() {
  while (true) {
    Timer timer;
    timer.start();
    MBED_ASSERT(encoder.update());
    timer.stop();
    std::string str;
    str = "Time taken to update encoder: " + std::to_string(timer.elapsed_time().count()) + "us\r\n";
    print(str);
    ThisThread::sleep_for(PERIOD);
  }
}

void Reader() {
  while (true) {
    std::string str;
    str = "Angle: " + std::to_string(encoder.getAngleDeg()) +
          ", Angular Velocity: " + std::to_string(encoder.getAngularVelocityDegPerSec()) + "\r\n";
    print(str);
    ThisThread::sleep_for(PERIOD);
  }
}

void print(const std::string &str) {
  std::unique_lock<Mutex> lock(mutex);
  printf("%s", str.c_str());
}

void callback() {
  std::string msg;
  msg = "Angle: " + std::to_string(encoder.getAngleDeg()) +
        ", Angular Velocity: " + std::to_string(encoder.getAngularVelocityDegPerSec()) + "\r\n";
  print(msg);
}