#pragma once
#include "mbed.h"
#include "CANMsg.h"


// gimbal::gimbal(){

// }
void periodic_10ms(){
    recieve_can_message();
}


gimbal::recieve_can_message(){
    CAN can(PB_8, PB_9); //Assuming rx and tx pins
    CANMsg  rxMsg;
    if (can.read(rxMsg)) {
            float percentage;
            rxMsg.getPayload(percentage); //ask how to parse can message
            position_in_percent = parsed_can_message;
            pitchServo.set_value(position * DEFAULT_RANGE);
    }
}
