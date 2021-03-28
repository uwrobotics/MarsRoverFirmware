#include "Logger.h"
#include "MAE3.h"

Encoder::MAE3 encoder({NC, 0});

Mutex mutex;

constexpr auto PERIOD = 100ms;

void Updater();
void Reader();
void print(const std::string &str);  // thread safe print

int main() {
  Thread updater_thread(osPriorityNormal), reader_thread(osPriorityNormal);

  updater_thread.start(&Updater);
  reader_thread.start(&Reader);

  updater_thread.join();
  reader_thread.join();
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
