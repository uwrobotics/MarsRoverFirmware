/**
 * This app tests the functionality of our PID library using the values stored in test_data.h
 * NOTE: This test-app is purely computational,
 * no motors or encoders are required
 */
#include <numeric>

#include <limits>

#include "PID.h"
#include "test_data.h"

constexpr float KP = 2, KI = 103, KD = 1;
constexpr float min_rpm   = -std::numeric_limits<float>::max(),
                max_rpm   = std::numeric_limits<float>::max();  // no saturation
constexpr float deadzone  = 0;
constexpr auto pid_period = 1ms;

constexpr float expected_avg_error       = 36.897f;
constexpr auto expected_avg_compute_time = 15us;

std::array<float, 5001> computed_control;
std::array<float, 5001> t;

int main() {
  printf("t,value\n");
  PID::Config config = {KP, KI, KD, min_rpm, max_rpm, deadzone, true, true};
  PID::PID controller(config);
  Timer perf_timer;
  Timer plot_timer;
  plot_timer.start();
  auto total_compute_time = 0us;
  float total_error       = 0;
  for (std::size_t i = 0; i < control.size(); i++) {
    /*if (i % 1000 == 0) {
      printf("Completed %zu /50001 iterations\r\n", i);
    }*/
    perf_timer.reset();
    perf_timer.start();
    float temp = controller.compute(setpoint.at(i), feedback.at(i));
    perf_timer.stop();
    computed_control[i] = temp;
    t[i] = chrono::duration_cast<chrono::duration<float>>(plot_timer.elapsed_time()).count();
    total_error += std::abs(control.at(i) - temp);
    perf_timer.stop();
    MBED_ASSERT(pid_period > perf_timer.elapsed_time());
    total_compute_time += perf_timer.elapsed_time();
    wait_us((pid_period - perf_timer.elapsed_time()).count());  // account for compute time
  }
  /*printf("TEST RESULTS\r\n");
  float average_error       = total_error / control.size();
  auto average_compute_time = total_compute_time / control.size();
  printf("Average difference between Matlab control signal and our control signal: %.3f\r\n", average_error);
  printf("Average time for a single call to the compute function: %llu us\r\n", average_compute_time.count());
  if (average_error - expected_avg_error > 0.001f) {
    printf("WARNING: Changes made to PID library have increased average error\r\n");
  }
  if (average_compute_time > expected_avg_compute_time) {
    printf("WARNING: Changes made to PID library have increased execution time of compute function\r\n");
  }*/
  for(uint64_t i = 0; i < 5001; i++) {
    printf("%.5f,%.5f\n", t[i], computed_control[i]);
  }
  while (true)
    ;
}
