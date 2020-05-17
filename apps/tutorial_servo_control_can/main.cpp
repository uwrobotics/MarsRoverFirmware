#include "mbed.h"
#include "TutorialServo/TutorialServo.h"

CAN canReceive(p30, p29);
CANMessage msg;
float percent

int main(){
    TutorialServo servo(PA_1);
    CANMessage msgRec;
     if(can.read(msgRec)) {
        rxMsg >> percent;
        servo.setPositionInPercent(percent)
    }
}
