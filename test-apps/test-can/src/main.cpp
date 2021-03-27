#include "CANConfig.h"
#include "CANInterface.h"
#include "Logger.h"
#include "hw_bridge.h"

CANInterface can(CANConfig::config);

int main() {
  Utility::logger << "";  // band-aid fix for logger bug
  printf("CAN test\r\n");

  HWBRIDGE::CANSignalValue_t rxSignal2Value = 0;
  HWBRIDGE::CANSignalValue_t rxSignal3Value = 0;
  uint16_t txSignalValue                    = 0;

  while (true) {
    // Read RX signals
    can.readStreamedSignal(HWBRIDGE::COMMON_TEST_MESSAGE2, HWBRIDGE::COMMON_TEST_SIGNAL2, rxSignal2Value);
    printf("RX test signal 2 value: %u\r\n", (uint16_t)rxSignal2Value);

    can.readStreamedSignal(HWBRIDGE::COMMON_TEST_MESSAGE2, HWBRIDGE::COMMON_TEST_SIGNAL3, rxSignal3Value);
    printf("RX test signal 3 value: %.6f\r\n", rxSignal3Value);

    // Update TX signals
    can.updateStreamedSignal(HWBRIDGE::COMMON_TEST_MESSAGE2, HWBRIDGE::COMMON_TEST_SIGNAL2, txSignalValue);
    txSignalValue = (txSignalValue + 1) % 4;

    can.updateStreamedSignal(HWBRIDGE::COMMON_TEST_MESSAGE2, HWBRIDGE::COMMON_TEST_SIGNAL3, 5.5f);

    ThisThread::sleep_for(1000ms);
  }
}

mbed_error_status_t handle_test_msg_one_shot(CANMsg& msg) {
  CANMsg msgACK;

  struct uwrt_mars_rover_can_common_test_message1_t msgStruct = {
      .common_test_signal1 = 0,
  };
  HWBRIDGE::CANMsgData_t msgData;
  uwrt_mars_rover_can_common_test_message1_pack(msgData.raw, &msgStruct, 1);

  msgACK.setID(HWBRIDGE::COMMON_TEST_MESSAGE1);
  msgACK.setPayload(msgData, UWRT_MARS_ROVER_CAN_COMMON_TEST_MESSAGE1_LENGTH);

  // Send a one shot back
  can.sendOneShotMessage(msgACK);

  return MBED_SUCCESS;
}
