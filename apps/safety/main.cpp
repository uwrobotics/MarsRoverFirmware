#include "mbed.h"

DigitalOut led1(LED1);

const int k_interval_ms = 500;

int main()
{
    while (true) {
        led1 = !led1;
        wait_ms(k_interval_ms);
    }
}
