#include "CANBus.h"
#include "CANMsg.h"
#include "TutorialServo.h"
#include "hw_bridge.h"
#include "mbed.h"

void send(const char *percentage);

CANBus can(PA_1, PA_0);
AnalogIn potVoltageIn(PA_0);
TutorialServo servo(PA_1);

constexpr uint16_t TX_ID = 0x100;
constexpr uint16_t RX_ID = 0x101;

int main() {
  CANMsg rxMsg;  // received CAN message

  while (1) {
    char percentage        = static_cast<uint8_t>(potVoltageIn.read());  // reading value of potentiometer
    const char *percentAdd = &percentage;

    send(percentAdd);  // sending the data

    if (can.read(rxMsg)) {
      printf("CAN message received\n");
      servo.setPositionInDegrees(percentage * servo.getServoRangeInDegrees());
    } else {
      printf("CAN message not received\n");
    }
  }

  return 0;
}

void send(const char *percentage) {
  CANMsg txMsg(static_cast<HWBRIDGE::CANID>(TX_ID), percentage, 8);  // message to be sent

  if (can.write(txMsg)) {
    printf("CAN message sent\n");  // if the message is transmitted successfully
  } else {
    printf("Transmission error\n");
  }
}