#include "TutorialServo.h"
#include "CANMsg.h"
#include "CANBus.h"
// mbed.h already included in TutorialServo.h

/*
CAN packet structure for servo
data 4-byte float put in via << overload
float represents desired angle
*/


CANBus can1(CAN_RX, CAN_TX);

int main() {
	TutorialServo Servo(PA_1, 180.0, 1, 2);
	CANMsg input_msg;
	float input_angle = 0;
	while (true) {
		if (can1.read(input_msg)) {
			input_msg >> input_angle; // extract the angle
			
			Servo.setPositionInDegrees(input_angle);
		}
	}
}