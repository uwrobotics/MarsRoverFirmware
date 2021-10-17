#include "TutorialServo.h"
#include "mbed.h"

//I wasn't sure which pin to read from, so I assumed PA_0)
CAN can2(PA_0, PA_1);

/*
Utilize the TutorialServo class that you wrote in the first part of the
challenge. Use the same pin (PA_1) as used earlier in the training module. The program should
be able to receive CAN messages which contain a percentage of the range of motion of the
servo as a float.
*/

TutorialServo servo = new TutorialServo(PA_1, 180, 1, 2);

int main() {
    CANMessage msg;
    while(1) {
        if(can2.read(msg)) {
            float degrees = 180 * msg;
            servo.setPositionInDegrees(degrees);
        }
    }
}