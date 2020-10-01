#include "UltraSonicSensor.h"
#include <cstdio>

// set up Ultrasonic sensor
UltraSonicSensor sensor(PA1, PA2);

int main(){

    while(1){
        // print out to the console
        printf("Distance: %f cm\n", sensor.getDistance());
        
    }

    return 0;
}