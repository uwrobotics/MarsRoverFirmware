
#include "mbed.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "TutorialServo.h"

CANBus can(PB_8, PB_9);
CANMsg rxMsg;
TutorialServo servo(PA_1, 180, 1, 2);

void messageCheck(void){ 				//payload is float between 0-1 which determines 					
	float data = 0;						//position of servo in range of 180 degrees

	if (can.read(rxMsg)) {
		rxMsg.getPayload(data);
		float degrees = data * 180;
		servo.setPositionInDegrees(degrees);
	}

}

int main(){

    while (true) {
		messageCheck();
    }
}