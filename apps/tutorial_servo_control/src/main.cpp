#include "mbed.h"
#include "TutorialServo.h"
#include "CANMsg.h"


CAN can(CAN1_RX, CAN1_TX);

int main() {

        TutorialServo servo(PA_1);
        CANMessage msg;

        while (true) {
                if(can.read(msg)){

                        printf("Message received: %d\n", msg.data[0]);
                        float range{servo.getRangeInDegrees()};
                        servo.setPositionInDegrees(range * msg.data[0]);
                }
        }
        return 0;
}

