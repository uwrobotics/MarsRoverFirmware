#include "mbed.h"
#include "TutorialServo.h"
#include "CANBus.h"
#include "CANMsg.h"

float servoAngle;
float percentage;

TutorialServo testServo(PA_1, 180.0, 1.0, 2.0);

// CANBus
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

int main() 
{
  
  CANMsg rxMsg;
  
  while (1)
  {
    if (can1.read(rxMsg))
    {
      rxMsg.getPayload(percentage);
      servoAngle = testServo.getServoRangeInDegrees()*percentage;
      testServo.setPositionInDegrees(servoAngle);
    }
    
    ThisThread::sleep_for(2ms);
  }
}
