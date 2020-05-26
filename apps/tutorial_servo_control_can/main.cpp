#include "mbed.h"
#include "TutorialServo/TutorialServo.h"
#include "CANMsg.h"

CAN canReceive(CAN_RX, CAN_TX);
CANMsg rxMsg;
float percent;
PwmOut servoPwmOut(PA_1);

int main(){
    TutorialServo servo(PA_1);
    servoPwmOut.period(0.020);
     if(canReceive.read(rxMsg)) {
        rxMsg >> percent;
        servo.setPositionInPercent(percent);
    }
}
