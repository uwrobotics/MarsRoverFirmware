#include "CANConfig.h"
#include "CANInterface.h"
#include "hw_bridge.h"
#include "TutorialServo.hpp"

TutorialServo servo(PA_1);
CANInterface can(CANConfig::config);

int main(){

  printf("TutorialServo test\r\n");
	
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  HWBRIDGE::CANSignalValue_t rxSignal1Value = 0;
  
  while (true){
    // Read RX signal
    can.getRXSignalValue(HWBRIDGE::CANID::COMMON_DEBUG_MESSAGE1, HWBRIDGE::CANSIGNAL::COMMON_DEBUG_SIGNAL1, rxSignal1Value);
    printf("RX debug signal value: %lu\r\n", static_cast<uint32_t>(rxSignal1Value));
	
    servo.setPositionInDegrees(static_cast<float>(rxSignal1Value));
    ThisThread::sleep_for(1000ms);
  }
}

// Receive a one-shot message (COMMON_DEBUG_MESSAGE3) and send a one-shot reply back
mbed_error_status_t handle_test_msg_one_shot(void) {
  CANMsg msgACK;

  // Initialize msg struct
  struct uwrt_mars_rover_can_common_debug_message3_t msgStruct = {
      .common_debug_signal3 = static_cast<uint8_t>(HWBRIDGE::COMMON_DEBUG_SIGNAL3_VALUES::DEBUG_VALUE_0),
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


