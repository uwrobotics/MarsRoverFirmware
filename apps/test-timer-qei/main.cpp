#include "HAL_PWM.h"
#include "HAL_QEI.h"
#include "mbed.h"
#include "stm32f4xx_hal.h"

Thread pinA_pwm_thread;
Thread pinB_pwm_thread;

// arbitrary digital pins. Needs to be different from what is being initialized in the QEI
DigitalOut pinA(PB_4);
DigitalOut pinB(PB_10);
DigitalOut led(LED1);
Serial pc(SERIAL_TX, SERIAL_RX, 9600);

float period       = 6;
float pulse_width  = 2;
float pulse_offset = pulse_width / 2;

void pinA_thread() {
  while (1) {
    pinA = 0;
    wait_ms(pulse_width);
    pinA = 1;
    wait_ms(period - pulse_width);
  }
}

void pinB_thread() {
  while (1) {
    pinB = 0;
    wait_ms(pulse_width);
    pinB = 1;
    wait_ms(period - pulse_width);
  }
}

int main() {
  led                     = 0;
  float expected_velocity = 0;  // will need to input some value

  pc.printf("Starting pwm generation");
  pinA_pwm_thread.start(pinA_thread);
  wait_ms(pulse_offset);
  pinB_pwm_thread.start(pinB_thread);

  HAL_QEI quadrature_encoder;

  while (1) {
    wait_ms(2000);

    pc.printf("Velocity: %f  CNT register=%lu dir: %u\n", quadrature_encoder.readEncoder(),
              quadrature_encoder.getTimerObject().Instance->CNT, quadrature_encoder.getDirection());

    if (expected_velocity == quadrature_encoder.getVelocity()) led = 1;
  }

  // pwm simulation testing
  // HAL_PWM pwm_timer;
  // while(1)
  // {
  //   wait_ms(2000);
  //   pwm_timer.calcDutyCycle();
  //   pc.printf("Period: %f  Pulse width: %f DutyCycle: %f\n", pwm_timer.getPeriod(), pwm_timer.getPulseWidth(),
  //             pwm_timer.getDutyCycle());
  // }
}