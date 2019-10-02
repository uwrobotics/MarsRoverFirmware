#include "mbed.h"
#include "CANMsg.h"
#include "rover_config.h"
#include "PinNames.h"
#include "tutorial_servo.h"

long DEFAULT_BAUD = 500000;
CAN can(PB_8, PB_9, DEFAULT_BAUD);
Serial pc(USBTX, USBRX, 9600);

CANMsg rxMsg;

TutorialServo servo(PA_1, 180);

uint8_t pos = 1;

int main(){

//    servo.setRangeInDegrees(180);

    while(1) {

        if(can.read(rxMsg)){

            rxMsg >> pos;
 //         servo.setPositionInDegrees(pos);
            pc.printf("%d \n", pos);
        }
    }
}
