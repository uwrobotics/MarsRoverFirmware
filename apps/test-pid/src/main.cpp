#include <numeric>
#include "PID.h"
#include "control.h"
#include "feedback.h"
#include "setpoint.h"

//#define DEBUG
#ifdef DEBUG
#define N 10
#endif

constexpr uint8_t KP      = 2;
constexpr uint8_t KI      = 103;
constexpr uint8_t KD      = 1;
constexpr int32_t min_rpm = INT32_MIN, max_rpm = INT32_MAX;  // 	-2,147,483,648 to 2,147,483,647
constexpr float deadzone  = 0;
constexpr auto pid_period = 1ms;  // 1kHz

#ifdef DEBUG
std::array<float, N> my_control;
#else
std::array<float, 5001> control_error;  // compare matlab's control signal to ours
#endif

int main() {
  printf("##################### PID TEST APP STARTED #####################\r\n");
  PID::Pid controller(KP, KI, KD, min_rpm, max_rpm, deadzone, false);
  Timer timer;
#ifdef DEBUG
  for (uint64_t i = 0; i < N; i++) {
#else
  for (uint64_t i = 0; i < control_error.size(); i++) {
#endif
    if(i%1000 == 0) printf("Completed %lld/50001 iterations\r\n", (long long)i);
    timer.reset();
    timer.start();
#ifdef DEBUG
    my_control[i] = controller.compute(setpoint.at(i), feedback.at(i));
#else
    control_error[i] = std::abs(control.at(i) - controller.compute(setpoint.at(i), feedback.at(i)));
#endif
    timer.stop();
    MBED_ASSERT(pid_period > timer.elapsed_time());
    wait_us((pid_period - timer.elapsed_time()).count());
    // ThisThread::sleep_for(1ms);
  }
  printf("ERROR STATISTICS\r\n");
#ifdef DEBUG
  for (uint64_t i = 0; i < N; i++) {
    printf("%.5f\r\n", my_control.at(i));
  }
#else
  float average = std::accumulate(control_error.begin(), control_error.end(), 0.0) / control_error.size();
  printf("Average difference between Matlab control signal and our control signal: %.3f\r\n", average);
#endif
  for (;;)
    ;
}