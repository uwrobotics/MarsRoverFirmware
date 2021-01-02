#include "TutorialServo.h"
#include "CANMsg.h"
// mbed.h already included in TutorialServo.h

/*
CAN packet structure for servo
data 4-byte float put in via << overload
float represents desired angle
*/

Ticker ticker;
CAN can1(p9, p10);

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