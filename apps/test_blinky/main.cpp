#include "mbed.h"
#include "servo.h"

int main()
{
    Servo my_servo(A1,LIMITED_SERVO,180.0,2.0,1.0);
    my_servo.move(50);

    while(1){}

    return 0;
}