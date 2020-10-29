#include "TutorialServo.h"
#include "CANBuffer.h"
#include "CANBus.h"
#include "hw_bridge.h"
#include "mbed.h"

// Interface and recieve buffer
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

void rxCANProcessor() {
  CANMsg rxMsg;

  while (true) {
    can1.read(rxMsg) 
    ThisThread::sleep_for(2ms);
  }
}

Thread rxCANProcessorThread(osPriorityAboveNormal);

// instantiating TutorialServo class
TutorialServo motor1 = TutorialServo(PA_1, 180, 1, 2); 

int main() {
printf("\r\n\r\n");
printf("TUTORIAL-SERVO-CAN-CONTROL APPLICATION STARTED\r\n");
printf("=======================\r\n");

// CAN init stuff
can1.setFilter(HWBRIDGE::CANFILTER::ROVER_CANID_FIRST_ARM_RX, CANStandard,
                 HWBRIDGE::ROVERCONFIG::ROVER_CANID_FILTER_MASK);
rxCANProcessorThread.start(rxCANProcessor);

float CAN_servo_float; 
CANMsg servo_msg; 

while(true){
// get CAN msg and save as a float value
if(can1.read(servo_msg)){
servo_float.getPayload(CAN_servo_float); 
} 

// translate the CAN msg value into degrees by multiplying the decimal by the total range of motion
float degree_input = CAN_servo_float*motor1.getServoRangeInDegrees(); 

// use this degree value to move the servo 
motor1.setPositionInDegrees(degree_input); 
ThisThread::sleep_for(2ms);
}

}
