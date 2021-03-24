#include "CANBus.h"
#include "CANConfig.h"
#include "Logger.h"
#include "hw_bridge.h"

CANBus can(CAN1_RX, CAN1_TX, &TestCAN::rxStreamedMsgMap, &TestCAN::txStreamedMsgMap, &TestCAN::rxOneShotMsgHandler,
           HWBRIDGE::ROVER_CANBUS_FREQUENCY_HZ);

int main() {
  printf("CAN test\r\n");
  while (true) {
    ThisThread::sleep_for(1000ms);
  }
}