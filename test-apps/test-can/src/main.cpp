#include "CANConfig.h"
#include "CANInterface.h"
#include "Logger.h"
#include "hw_bridge.h"

CANInterface can(CANConfig::config);

int main() {
  Utility::logger << "";  // Band-aid fix for logger bug (issue #328)
  printf("CAN test\r\n");

  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  HWBRIDGE::CANSignalValue_t rxSignal1Value = 0;
  HWBRIDGE::CANSignalValue_t txSignal1Value = 0;

  while (true) {
    // Read RX signal
    can.readStreamedSignal(HWBRIDGE::CANID::COMMON_DEBUG_MESSAGE1, HWBRIDGE::CANSIGNAL::COMMON_DEBUG_SIGNAL1,
                           rxSignal1Value);
    printf("RX debug signal value: %lu\r\n", (uint32_t)rxSignal1Value);

    // Update TX signal
    can.updateStreamedSignal(HWBRIDGE::CANID::COMMON_DEBUG_MESSAGE2, HWBRIDGE::CANSIGNAL::COMMON_DEBUG_SIGNAL2,
                             txSignal1Value);
    printf("TX test signal 2 value: %u\r\n", (uint16_t)txSignal1Value);
    txSignal1Value = ((uint16_t)txSignal1Value + 1) % 8;

    ThisThread::sleep_for(1000ms);
  }
}

// Receive a one-shot message (COMMON_DEBUG_MESSAGE3) and send a one-shot reply back
mbed_error_status_t handle_test_msg_one_shot(CANMsg& msg) {
  CANMsg msgACK;

  // Initialize msg struct
  struct uwrt_mars_rover_can_common_debug_message3_t msgStruct = {
      .common_debug_signal3 = (uint8_t)HWBRIDGE::COMMON_DEBUG_SIGNAL3_VALUES::COMMON_DEBUG_SIGNAL3_DEBUG_VALUE_0,
  };

  // Pack msg struct contents into raw bytes
  HWBRIDGE::CANMsgData_t msgData;
  uwrt_mars_rover_can_common_debug_message3_pack(msgData.raw, &msgStruct, 1);

  // Prepare CANMsg
  msgACK.setID(HWBRIDGE::CANID::COMMON_DEBUG_MESSAGE3);
  msgACK.setPayload(msgData, UWRT_MARS_ROVER_CAN_COMMON_DEBUG_MESSAGE3_LENGTH);

  // Send a one shot back
  can.sendOneShotMessage(msgACK, 1ms);

  printf("One-shot reply sent\r\n");

  return MBED_SUCCESS;
}
