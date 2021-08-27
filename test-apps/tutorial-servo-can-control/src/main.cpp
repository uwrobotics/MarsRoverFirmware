#include "CANMsg.h"
#include "TutorialServo.h"

TutorialServo servo(PA_1);
CAN           can(PB_8, PB_9);
CANMsg        rxMsg;
float         percent;

int main() {
    while (1) {
        if (can.read(rxMsg)) {
            rxMsg.getPayload(percent);
            servo.setPositionInDegrees(percent * servo.getServoRangeInDegrees());
        }
    }
}