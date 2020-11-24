#include "TutorialServo.h"
#include "mbed.h"
#include "CANMsg.h"
#include <cstdlib>


TutorialServo tutorialServo(PA_1);
CANBus can1(CAN1_RX, CAN1_TX);
std::Thread rxCANProcessorThread(osPriorityAboveNormal);

//Servo position update function for thread
void updateServoPositon();


int main() {
    printf("Tutorial Servo App Start\n");
    printf("=======================\r\n");

    // Initialize CAN thread
    rxCANProcessorThread.start(updateServoPosition);

    // Pretty sure this is required or the program will end and stop the CAN thread
    while (true) {} 
}


// Since the arm app used threads I attempted to use it too,
// but for this application probably would just put it in the main loop
void updateServoPosition() {
    CANMsg rxMsg;
    float rxPercentage {0};
    while (true) {
        if (can1.read(rxMsg)) {
            rxMsg >> rxPercentage;
            tutorialServo.setPositionInPercentage(rxPercentage);
        }
        ThisThread::sleep_for(2ms);
    }
}
