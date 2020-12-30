/**
 * This app tests the functionality of our PID library
 * It uses the setpoint and feedback values defined in setpoint.h and feedback.h respectively
 * Next, the control signal generated by our PID library is compared to that in control.h
 * The data found in the header files was generated using a MatLab script
 * NOTE: This test-app is purely computational, no motors or encoders are required
 */

#include <numeric>
#include "PID.h"
#include "control.h"
#include "feedback.h"
#include "setpoint.h"

constexpr uint8_t KP      = 2;
constexpr uint8_t KI      = 103;
constexpr uint8_t KD      = 1;
constexpr int32_t min_rpm = INT32_MIN, max_rpm = INT32_MAX;  // no saturation
constexpr float deadzone  = 0;
constexpr auto pid_period = 1ms;

std::array<float, 5001> control_error;  // compare matlab's control signal to ours

int main() {
  printf("##################### PID TEST APP STARTED #####################\r\n");
  PID::Pid controller(KP, KI, KD, min_rpm, max_rpm, deadzone, false);
  Timer timer;
  for (uint64_t i = 0; i < control_error.size(); i++) {
    if (i % 1000 == 0) {
      printf("Completed %lld/50001 iterations\r\n", (long long)i);
    }
    timer.reset();
    timer.start();
    control_error[i] = std::abs(control.at(i) - controller.compute(setpoint.at(i), feedback.at(i)));
    timer.stop();
    MBED_ASSERT(pid_period > timer.elapsed_time());
    wait_us((pid_period - timer.elapsed_time()).count());  // account for compute time
  }
  printf("ERROR STATISTICS\r\n");
  float average = std::accumulate(control_error.begin(), control_error.end(), 0.0) / control_error.size();
  printf("Average difference between Matlab control signal and our control signal: %.3f\r\n", average);
  while (true)
    ;
}
