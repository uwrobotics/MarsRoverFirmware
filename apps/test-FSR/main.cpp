#include "mbed.h"

AnalogIn fsr(A0);

int main(){
    while (1){
        wait_ms(100);
        printf("%f", fsr);
    }
}