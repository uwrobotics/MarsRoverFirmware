/* mbed Microcontroller Library
 */

//#include "SystemReport.h"
#include "mbed.h"
#include "TutorialServo.h"
#include "CANMsg.h"

CAN can(D3, D4);
CANMsg msg;
uint8_t convertedMessage;
TutorialServo servo(PA_1, 50);

// main() runs in its own thread in the OS

int main() 
{
  while(true)
  {
    if(can.read(msg))
    {
      msg >> convertedMessage;
      servo.setPositionInDegrees(convertedMessage);
    }
  }
}
