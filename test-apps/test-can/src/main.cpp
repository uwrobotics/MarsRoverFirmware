#include "CANConfig.h"
#include "CANInterface.h"
#include "Logger.h"
#include "hw_bridge.h"

CANInterface can(CANConfig::config);

int main() {
  Utility::logger << "";  // Band-aid fix for logger bug (issue #328)
  printf("CAN test\r\n");

  // Set hw filter
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  HWBRIDGE::CANSignalValue_t rxSignal2Value = 0;
  HWBRIDGE::CANSignalValue_t rxSignal3Value = 0;
  HWBRIDGE::CANSignalValue_t txSignal2Value = 0;
  HWBRIDGE::CANSignalValue_t txSignal3Value = 5.5f;

  while (true) {
    // Read RX signals
    can.readStreamedSignal(HWBRIDGE::CANID::COMMON_TEST_MESSAGE2, HWBRIDGE::CANSIGNAL::COMMON_TEST_SIGNAL2,
                           rxSignal2Value);
    printf("RX test signal 2 value: %u\r\n", (uint16_t)rxSignal2Value);

    can.readStreamedSignal(HWBRIDGE::CANID::COMMON_TEST_MESSAGE2, HWBRIDGE::CANSIGNAL::COMMON_TEST_SIGNAL3,
                           rxSignal3Value);
    printf("RX test signal 3 value: %.6f\r\n", rxSignal3Value);

    // Update TX signals
    can.updateStreamedSignal(HWBRIDGE::CANID::COMMON_TEST_MESSAGE2, HWBRIDGE::CANSIGNAL::COMMON_TEST_SIGNAL2,
                             txSignal2Value);
    printf("TX test signal 2 value: %u\r\n", (uint16_t)txSignal2Value);
    txSignal2Value = ((uint16_t)txSignal2Value + 1) % 4;

    can.updateStreamedSignal(HWBRIDGE::CANID::COMMON_TEST_MESSAGE2, HWBRIDGE::CANSIGNAL::COMMON_TEST_SIGNAL3,
                             txSignal3Value);
    printf("TX test signal 3 value: %.3f\r\n", txSignal3Value);

    ThisThread::sleep_for(1000ms);
  }
}

// Receive a one-shot message (COMMON_TEST_MESSAGE1) and send a one-shot reply back
mbed_error_status_t handle_test_msg_one_shot(CANMsg& msg) {
  CANMsg msgACK;

  // Initialize msg struct
  struct uwrt_mars_rover_can_common_test_message1_t msgStruct = {
      .common_test_signal1 = (uint8_t)HWBRIDGE::COMMON_TEST_SIGNAL1_VALUES::COMMON_TEST_SIGNAL1_TEST_VALUE_0,
  };

  // Pack msg struct contents into raw bytes
  HWBRIDGE::CANMsgData_t msgData;
  uwrt_mars_rover_can_common_test_message1_pack(msgData.raw, &msgStruct, 1);

  // Prepare CANMsg
  msgACK.setID(HWBRIDGE::CANID::COMMON_TEST_MESSAGE1);
  msgACK.setPayload(msgData, UWRT_MARS_ROVER_CAN_COMMON_TEST_MESSAGE1_LENGTH);

  // Send a one shot back
  can.sendOneShotMessage(msgACK, 1ms);

  printf("One-shot reply sent\r\n");

  return MBED_SUCCESS;
}
