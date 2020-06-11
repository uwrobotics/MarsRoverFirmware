#include "HAL_QEI.h"
#include "mbed.h"
#include "stm32f4xx_hal.h"

PwmOut pinA(PA_7);
PwmOut pinB(PA_6);
DigitalOut led(LED1);
Serial pc(SERIAL_TX, SERIAL_RX, 9600);

int main() {
  led                     = 0;
  float expected_period   = 0.004;
  float expected_velocity = 0;  // will need to input some value

  pc.printf("Starting pwm generation");
  pinA.period(expected_period);
  wait_ms(2);
  pinB.period(expected_period);

  HAL_QEI quadrature_encoder;

  while (1) {
    wait_ms(2000);
    pc.printf("Velocity: %f  CNT register=%lu dir: %d\n", quadrature_encoder.readEncoder(), TIM2->CNT, quadrature_encoder.getDirection()) ;

    if (expected_velocity == quadrature_encoder.getVelocity()) {
      led = 1;
    }
  }
}