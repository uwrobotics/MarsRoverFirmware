#include "Quadrature64CPR.h"
#include "Logger.h"

// first pin is yellow wire
// second pin is white wire
constexpr Encoder::Quadrature64CPR::Config config = {PA_5, PA_6, NC, 0};
Encoder::Quadrature64CPR enc(config);

// Timers
Timer theta_timer, theta_dot_timer;

// Variables for angle and speed
float theta     = 0;
float theta_dot = 0;

// Synchronous testing
void Updater();
void Reader();

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
  print("Updating...");
  ThisThread::sleep_for(PERIOD);
}

void Reader() {
  print("Reading...");
  ThisThread::sleep_for(PERIOD);
}

void print(const std::string &str) {
  std::unique_lock<Mutex> lock(print_mutex);
  printf("%s", str.c_str());
}

  // while (true) {
  //   // reset the timers
  //   theta_timer.reset();
  //   theta_dot_timer.reset();
  //   // update the encoder
  //   if (enc.update()){
  //     // read and print the angle
  //     theta = enc.getAngleDeg();
  //     theta_timer.stop();
  //     printf("Angle reading took: %lluus\tAngle: %.3f\r\n", theta_timer.elapsed_time().count(), theta);
  //     // read and print the speed
  //     theta_dot = enc.getAngularVelocityDegPerSec();
  //     theta_dot_timer.stop();
  //     printf("Angular speed reading took: %lluus\tSpeed: %.3f\r\n", theta_dot_timer.elapsed_time().count(), theta_dot);
  //   }else {
  //     printf("Error updating the encoder\n");
  //   }
  //   ThisThread::sleep_for(500ms);
  // }