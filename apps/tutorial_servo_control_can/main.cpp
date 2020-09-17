#include <cstdio>

#include "CAN.h"
#include "CANMsg.h"
#include "TutorialServo.h"
#include "hw_bridge.h"
#include <cstdio>


TutorialServo servo_can(PA_1);  // create servo

// initialize CAN

CANMsg rxMsg;
CANMsg txMsg;
#define TX_ID 0x100
#define RX_ID 0x101
CAN can(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

int main(){
  float percent;
  rxMsg.id = RX_ID;  // set message ID

  while (1) {

    if (can.read(rxMsg)) {
  
      printf("-------------------------------------\r\n");
      printf("CAN message received\r\n");
    

      // Filtering performed by software:
      if (rxMsg.id == RX_ID) {
        rxMsg >> percent;  // extract data from the received CAN message
        
        servo_can.setServoPositionWithPercent(percent);

      }
 
    }

  }

  return 0;
}
