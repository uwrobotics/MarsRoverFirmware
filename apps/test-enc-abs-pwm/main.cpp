#include "EncoderAbsolute_PWM.h"
#include "mbed.h"

Encoder::t_encoderConfig config = {
    // PWM encoder pins
    .pin_PWM = ENC_PWM_TRNTBL,

    .degreesPerUnit = 360.0,
    .inverted       = false};

EncoderAbsolute_PWM encoder(config);

// Wire the output PWM signal to the PWM input
// or connect external PWM input to pwmIn pin
PwmOut pwmOut(PinName::MTR_PWM_TRNTBL);

Timer timer;
Timer printTimer;

DigitalOut led(LED1);

int main() {
  float inverter = +1;
  int period_ms  = 4;  // Equivalent to 0.5kHz frequency
  float duty     = 0.1;

  // Specify PWM period
  pwmOut.period_ms(period_ms);

  // START THE TIMER
  timer.start();
  printTimer.start();

  while (true) {
    led = !led;

    // Sweep the output duty cycle at 2%/second (7.2 deg / sec)
    if (timer.read() >= 0.005) {
      // Set the duty cycle on the pins
      duty += 0.0001 * inverter;
      pwmOut.write(duty);

      if (duty <= 0.1) {
        inverter = 1.0;
      } else if (duty >= 0.9) {
        inverter = 0.0;  // Hold at 0.9 duty cycle for 5 seconds
        if (timer.read() >= 5) {
          inverter = -1.0;
          timer.reset();
        }
      } else {
        timer.reset();
      }
    }

    if (printTimer.read() >= 0.05) {
      printTimer.reset();
      printf("Angle: %f, Angular Velocity :%f\r\n", encoder.getAngle_Degrees(), encoder.getVelocity_DegreesPerSec());
    }
  }
}
