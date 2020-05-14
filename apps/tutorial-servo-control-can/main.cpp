#include "mbed.h"
#include "TutorialServo.h"
#include "PwmIn.h"
#include "CANMsg.h"

// Write a program to control a servo via CAN. Utilize the TutorialServo class that you wrote in the first part of the challenge. 
// Use the same pin (PA_1) as used earlier in the training module. 
// The program should be able to receive CAN messages which contain a percentage of the range of motion of the servo as a float. 

#define SET_SERVO_RANGE 0x123 //ID

float percentOfRange = 0;

CAN can1(CAN_RX, CAN_TX, 500000); //(CANrx, CANtx, rover_bus_frequency), CAN_RX, CAN_TX are mapped elsewhere

TutorialServo myservo(PA_1);


static setPosition(CANMsg msg)
{
	msg.getPayload(percentOfRange) //percentOfRange will get val assigned by the function, its PBR (see getPayload func)
	

	switch(msg.id)
	{
		case SET_SERVO_RANGE:
			myservo.setPositionInDegrees(percentOfRange);
	}
}


int main()
{
	CANMsg msgrx;
	
	
	while (true)
	{
		if (can1.read(msgrx))
		{
			setPosition(msgrx);
		}
		else
			printf ("Message ID not found!");
	}
}
