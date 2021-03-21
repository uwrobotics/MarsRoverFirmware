
#include "TutorialServo.h"
#include "CANBuffer.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "hw_bridge.h"
#include "mbed.h"

TutorialServo servo1 = TutorialServo(PA_1);

CANBus can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
CANMsg servoMsg;

int main() {

	while(true) {
		if(can.read(servoMsg)) {
			float degrees;
			const float PERCENT_TO_DEG = 3.6;
			servoMsg.getPayload(degrees);

			servo1.setPositionInDegrees(degrees * PERCENT_TO_DEG);
		}
		ThisThread::sleep_for(2ms);
	}
}
