#include "CANBuffer.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "mbed.h"
#include "TutorialServo.h"

// The program should
// be able to receive CAN messages which contain a percentage of the range of motion of the
// servo as a float.

float percentageDegrees = 0;
//Tutorial Servo object set to pin PA_1
TutorialServo myServo(PA_1);
//Initialize CAN interface and set frequency 
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);


 void rxCANProcessor() {
  CANMsg rxMsg;

  while (true) {
    if (can1.read(rxMsg)) {
      // read the CAN message payload 
        //Put what you're actually going to do with the CAN message
        
              // extract data from the received CAN message 
              // in the same order as it was added on the transmitter side
              //Use pyaload function instesad of right shift
              rxMsg.getPayload(percentageDegrees);
              float actualDegrees = percentageDegrees*myServo.getServoRangeInDegrees();
              myServo.setPositionInDegrees(actualDegrees);
              // rxMsg >> percentageDegrees; 
              // myServo.setPositionInDegrees((m_servoRangeInDegrees*percentageDegrees));   
              // pc.printf("  percentageDegrees = %e V\r\n", percentageDegrees);
    }
  ThisThread::sleep_for(2ms);
  }
}
 
//use compiler specified in readme. it's specific to code relating to arm
//Commit changes to your forked repo and submit a pull request 
//      to the actual Mars Rover repo so your code can be reviewed
//Create thread
Thread rxCANProcessorThread(osPriorityAboveNormal);
//when you commit the files you modified, the 60+ untracked changes will not matter anymore
//make sure you push your changes to the forked version of the MARS Rover repo and not the actual one
int main() {
// wait for can message to be received -
// process the CAN message and move the servo
// payload is the actual thing you are sending over CAN
// find out why you have 65 changes
  rxCANProcessorThread.start(rxCANProcessor);
  //rxCANProcessor(CAN1_RX);
}