#include "CANMsg.h"
#include "hw_bridge.h"
#include "mbed.h"

DigitalOut led1(LED1);

constexpr auto k_interval = 500ms;

CAN can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

int main() {
  can.filter(HWBRIDGE::CANFILTER::ROVER_CANID_FIRST_SCIENCE_RX, HWBRIDGE::ROVERCONFIG::ROVER_CANID_FILTER_MASK,
             CANStandard);
  while (true) {
    led1 = !led1;
    ThisThread::sleep_for(k_interval);
  }
}
