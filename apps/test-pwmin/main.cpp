#include "mbed.h"
#include "PwmIn.h"

Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut led(LED1);

// Wire the output PWM signal to the PWM input
PwmOut pwmOut(PinName::MTR_PWM_SHLDR);
PwmIn pwmIn(PinName::ENC_PWM_SHLDR, 50);

Timer timer;


int main() {

    float inverter = +1;
    int period_ms = 2; // Equivalent to 0.5kHz frequency
    float duty = 0.0f;

    // Specify PWM period
    pwmOut.period_ms(period_ms);

    // START THE TIMER
    timer.start();

    while (true) {
        led = !led;

        // Sweep the output duty cycle at 1%/second (3.6 deg / sec)
        if (timer.read() >= 0.01) {
            timer.reset();

            // Set the duty cycle on the pins
            duty += 0.0001 * inverter;
            pwmOut.write(duty);

            if (duty <= 0.0) {
                inverter = 1.0;
            } else if (duty >= 1.0) {
                inverter = -1.0;
            }
        }

        pc.printf("Avg PW: %+f, \tAvg Prd: %+f, \tRaw Duty: %+f, \tAvg Duty: %+f, \tAvg Duty Velo: %+f, \tAvg Ang Velo: %+f\r\n", 
                  pwmIn.avgPulseWidth(), pwmIn.avgPeriod(), pwmIn.dutyCycle(), pwmIn.avgDutyCycle(), pwmIn.avgDutyCycleVelocity(), pwmIn.avgDutyCycleVelocity() * 360.0);

    }
}