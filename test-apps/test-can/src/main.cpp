#include "CANAutogenWrapper.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "Logger.h"
#include "hw_bridge.h"

CANSignalMap rxSignalMap = {
    // Msg 1
    {UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID,
     {
         {HWBRIDGE::COMMON_CAN_BUS_ID, UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_COMMON_CAN_BUS_ID_SNA_CHOICE},
     }},

    // Msg 2
};

int main() {
  printf("CAN test\r\n");
  while (true) {
  }
}