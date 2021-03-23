#include "CANAutogenWrapper.h"

// --- Message packer function prototypes ---
static bool common_switch_can_bus_frame_id_packer(uint8_t* raw, const CANSignalMap* signalMap);

// --- Message unpacker function prototypes ---
static bool common_switch_can_bus_frame_id_unpacker(uint8_t* raw, CANSignalMap* signalMap);

// Message packers LUT
typedef bool (*msgPacker)(uint8_t* raw, const CANSignalMap* signalMap);
static const Utility::LookupTable<HWBRIDGE::CANMsgID_t, msgPacker,
                                  +[](uint8_t* raw, const CANSignalMap* signalMap) -> bool { return false; }>
    msgPackerLUT = {{UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID, common_switch_can_bus_frame_id_packer}};

// Message unpackers LUT
typedef bool (*msgUnpacker)(uint8_t* raw, CANSignalMap* signalMap);
static const Utility::LookupTable<HWBRIDGE::CANMsgID_t, msgUnpacker,
                                  +[](uint8_t* raw, CANSignalMap* signalMap) -> bool { return false; }>
    msgUnpackerLUT = {{UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID, common_switch_can_bus_frame_id_unpacker}};

bool packCANMsg(uint8_t* raw, HWBRIDGE::CANMsgID_t msgID, CANSignalMap* signalMap) {
  return msgPackerLUT.at(msgID)(raw, signalMap);
}

bool unpackCANMsg(uint8_t* raw, HWBRIDGE::CANMsgID_t msgID, CANSignalMap* signalMap) {
  return msgUnpackerLUT.at(msgID)(raw, signalMap);
}

// --- Message packer funciton definitions ---

// Message: COMMON_switchCANBusFrameID
static bool common_switch_can_bus_frame_id_packer(uint8_t* raw, const CANSignalMap* signalMap) {
  bool retval = false;
  struct uwrt_mars_rover_can_common_switch_can_bus_t msgStruct;

  if (signalMap->find(UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID) != signalMap->end()) {
    const CANSignalLUT* signals = &(signalMap->at(UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID));

    for (auto it = signals->begin(); it != signals->end(); it++) {
      HWBRIDGE::CANSIGNALNAME signalName     = it->first;
      HWBRIDGE::CANSignalValue_t signalValue = it->second;

      switch (signalName) {
        case HWBRIDGE::CANSIGNALNAME::COMMON_CAN_BUS_ID:
          msgStruct.common_can_bus_id = uwrt_mars_rover_can_common_switch_can_bus_common_can_bus_id_encode(signalValue);
          break;
        default:
          retval = false;
          break;
      }
    }
    retval = (uwrt_mars_rover_can_common_switch_can_bus_pack(raw, &msgStruct,
                                                             UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_LENGTH) ==
              UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_LENGTH);
  }
  return retval;
}

// --- Message unpacker function definitions ---

// Message: COMMON_switchCANBusFrameID
static bool common_switch_can_bus_frame_id_unpacker(uint8_t* raw, CANSignalMap* signalMap) {
  bool retval = false;
  struct uwrt_mars_rover_can_common_switch_can_bus_t msgStruct;

  retval = (uwrt_mars_rover_can_common_switch_can_bus_unpack(&msgStruct, raw,
                                                             UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_LENGTH) == 0);

  if (signalMap->find(UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID) != signalMap->end()) {
    CANSignalLUT* signals = &(signalMap->at(UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID));

    for (auto it = signals->begin(); it != signals->end(); it++) {
      HWBRIDGE::CANSIGNALNAME signalName = it->first;

      switch (signalName) {
        case HWBRIDGE::CANSIGNALNAME::COMMON_CAN_BUS_ID:
          (*signals)[signalName] =
              uwrt_mars_rover_can_common_switch_can_bus_common_can_bus_id_decode(msgStruct.common_can_bus_id);
          break;
        default:
          retval = false;
          break;
      }
    }
  } else {
    retval = false;
  }

  return retval;
}
