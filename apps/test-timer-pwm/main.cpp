#include "HAL_PWM.h"
#include "mbed.h"
#include "stm32f4xx_hal.h"

PwmOut pinA(PA_6);
DigitalOut led(LED1);
Serial pc(SERIAL_TX, SERIAL_RX, 9600);

int main() {
  led = 0;
  pc.printf("Starting pwm generation");

  float expected_period     = 4;
  float expected_dutyCycle  = 0.50f;
  float expected_pulseWidth = 0;

  pinA.period_ms(expected_period);
  pinA.write(expected_dutyCycle);  // % duty cycle

  HAL_PWM pwm_Timer;

  while (1) {
    wait_ms(2000);

    pwm_Timer.calcDutyCycle();
    pc.printf("Period: %f  Pulse width: %f DutyCycle: %f\n", pwm_Timer.getPeriod(), pwm_Timer.getPulseWidth(),
              pwm_Timer.getDutyCycle());

    if (expected_period == pwm_Timer.getPeriod() && expected_dutyCycle == pwm_Timer.getDutyCycle()) {
      led = 1;
    }
  }
}
