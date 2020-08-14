#include "mbed.h"
#include "CAN.h"
#include "TutorialServo.h"
#include "CANMsg.h"
#include "hw_bridge.h"
void controlServo();
float canMessengerServo();



//set pins 
AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);
//CAN setup
CAN can(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
CANMsg rxMsg;
CANMsg txMsg;
#define TX_ID 0x100
#define RX_ID 0x101




int main() {
  float percent;
 
  TutorialServo servo_can(PA_1);	//create servo object attached to pin PA_1
  txMsg.id = TX_ID;                 //set message ID   
  
  
  txMsg << percent;

  
  while (1)
    if (can.read(rxMsg)) {
  
  printf("-------------------------------------\r\n");
  printf("CAN message received\r\n");
  printMsg(rxMsg);

  // Filtering performed by software:
  if (rxMsg.id == RX_ID) {
    rxMsg >> percent;  // extract data from the received CAN message
    printf("  percent = %f\r\n", counter);

     servo_can.setServoPositionWithPercent(percent);

  }	
 

  
  return 0;
}



void controlServo() {
//set servo frequency
	servoPwmOut.period(0.20);
	while (1) {
        float potVoltage = potVoltageIn.read();
		// use equation of a line to map potvoltage to pulsewidth
          servoPwmOut.pulsewidth(((0.001/3.3)*potVoltage)+0.001);	
  
	}
	
}

