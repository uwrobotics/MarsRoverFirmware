#include "mbed.h"
#include "TutorialServo.h"
#include "CANBus.h"
#include "CANMsg.h"

float servoAngle;
float percentage;

TutorialServo testServo(PA_1, 180.0, 1.0, 2.0);

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

Thread rxCANProcessorThread(osPriorityAboveNormal);

int main() 
{
  can1.setFilter(HWBRIDGE::CANFILTER::ROVER_CANID_FIRST_ARM_RX, CANStandard,
                 HWBRIDGE::ROVERCONFIG::ROVER_CANID_FILTER_MASK);
  
  rxCANProcessorThread.start(rxCANProcessor);
  while (1)
  {
    if (can1.read(rxMsg))
    {
      rxMsg >> percentage
      servoAngle = testServo.getServoRangeInDegrees()*percentage;
      testServo.setPositionInDegrees(servoAngle);
    }
    
    ThisThread::sleep_for(1ms);
  }
}
