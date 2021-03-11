#include "mbed.h"
#include "TutorialServo.h"
#include "CANBus.h"
#include "CANMsg.h"

float servoAngle;

// CANBus
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

void rxCANProcessor() 
{
  CANMsg rxMsg;
  
  while (true)
  {
    if (can1.read(rxMsg))
    {
      canHandlerMap.at(rxMsg.getID())(rxMsg);
    }
    ThisThread::sleep_for(2ms);
  }
}

int main() 
{
  
}
