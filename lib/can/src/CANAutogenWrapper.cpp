#include "CANAutogenWrapper.h"

void packCANMsg(uint8_t* raw, uint16_t msgID, const std::vector<CANSignal>* signals) {
  switch (msgID) {
    case UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID:
      struct uwrt_mars_rover_can_common_switch_can_bus_t msgStruct;
      for (CANSignal signal : *signals) {
        switch (signal.getName()) {
          case CAN_SIGNAL_NAME::COMMON_CAN_BUS_ID:
            msgStruct.common_can_bus_id =
                uwrt_mars_rover_can_common_switch_can_bus_common_can_bus_id_encode(signal.getValue());
            break;
          default:
            break;
        }
      }
      uwrt_mars_rover_can_common_switch_can_bus_pack(raw, &msgStruct, UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_LENGTH);
      break;

    default:
      break;
  }
}
