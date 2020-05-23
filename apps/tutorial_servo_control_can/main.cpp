#include "mbed.h"
#include "TutorialServo.h"
#include "CANMsg.h"

CAN canReceive(p30, p29);
CANMsg rxMsg;
float percent;

int main(){
    TutorialServo servo(PA_1);
    servo.period(0.020);
    CANMessage msgRec;
     if(can.read(msgRec)) {
        rxMsg >> percent;
        servo.setPositionInPercent(percent);
    }
}
