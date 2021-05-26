#include "TutorialServo.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "mbed.h"

int main()
{
    CANBus can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
    CANMsg rxMsg;
    float rangeOfMotion = NULL;

    //listens on can for any incoming can messages and if any places into rxMsg object
    while(true)
    {
        if(can.read(rxMsg))
        {
            rxMsg.getPayload(rangeOfMotion);
        }
    }

    TutorialServo servoObject;
    //controls the servo using float value stored in CAN message.
    servoObject TutorialServo(PA_1, rangeOfMotion);

}

