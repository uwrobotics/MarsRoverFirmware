#include "CANMsg.h"
#include "can_config.h"
#include "mbed.h"
#include "rover_config.h"

DigitalOut led1(LED1);

const int k_interval_ms = 500;

CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);

int main() {
  can.filter(ROVER_CANID_FIRST_SCIENCE_RX, ROVER_CANID_FILTER_MASK, CANStandard);
  while (true) {
    led1 = !led1;
    wait_ms(k_interval_ms);
  }
}
