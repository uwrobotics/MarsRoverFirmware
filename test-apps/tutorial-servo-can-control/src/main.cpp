#include "CANMsg.h"
#include "mbed.h"
#include "TutorialServo.h"

TutorialServo servo(PA_1); //initialize servo variable from class we created using appropriate pin
CAN can(PB_8, PB_9); //rx and tx pins
CANMsg  rxMsg;


int main() {
    float  percentage;
    while (1) {
        if (can.read(rxMsg)) {
            rxMsg.getPayload(percentage);
            servo.setPositionInDegrees(percentage * servo.getServoRangeInDegrees());
        }
    }
}