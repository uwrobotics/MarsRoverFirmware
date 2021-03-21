#include "Logger.h"
#include "MAE3.h"

Encoder::MAE3 encoder({NC, 0});

// Wire the output PWM signal to the PWM input
// or connect external PWM input to pwmIn pin
PwmOut pwmOut(NC);

Timer timer;
Timer printTimer;

DigitalOut led(LED1);

int main() {
  float inverter = +1;
  auto period    = 4ms;  // Equivalent to 0.5kHz frequency
  float duty     = 0.1;

  // Specify PWM period
  pwmOut.period(std::chrono::duration_cast<std::chrono::duration<float>>(period).count());

  // START THE TIMER
  timer.start();
  printTimer.start();

  while (true) {
    led = !led;

    // Sweep the output duty cycle at 2%/second (7.2 deg / sec)
    if (timer.elapsed_time() >= 5ms) {
      // Set the duty cycle on the pins
      duty += 0.0001 * inverter;
      pwmOut.write(duty);

      if (duty <= 0.1) {
        inverter = 1.0;
      } else if (duty >= 0.9) {
        inverter = 0.0;  // Hold at 0.9 duty cycle for 5 seconds
        if (timer.elapsed_time() >= 5s) {
          inverter = -1.0;
          timer.reset();
        }
      } else {
        timer.reset();
      }
    }

    if (printTimer.elapsed_time() >= 50ms) {
      MBED_ASSERT_WARN(encoder.update())
      printTimer.reset();

      printf("Angle: %f, Angular Velocity :%f\r\n", encoder.getAngleDeg(), encoder.getAngularVelocityDegPerSec());
    }
  }
}
