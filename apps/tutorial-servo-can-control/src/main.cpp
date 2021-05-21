#include "mbed.h"
#include "CANMsg.h"
#include "TutorialServo.h"

PinName servoPin(PA_1);
CAN     can(PB_8, PB_9);  // assume rx_pin:PB_8, tx_pin: PB_9

int main(){
    // initializtion
    TutorialServo servo_out(servoPin);
    float rec_can;
    float output_degree;

    can.frequency(1000000); // set CAN bit rate to 1Mbps
    CANMsg rxMsg;

    // loop
    while(1){
        if (can.read(rxMsg)) {
            rxMsg.getPayload(rec_can);
            output_degree = rec_can * (servo_out.getServoRangeInDegrees());
            servo_out.setPositionInDegrees(output_degree);
        }
    }
}
