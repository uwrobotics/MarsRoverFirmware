#include "CANConfigArm.h"
#include "Logger.h"

CANInterface can(CANConfig::config);

int main() {
  Utility::logger << "";  // Band-aid fix for logger bug (issue #328)

  printf("\r\n\r\n");
  printf("STRESS CAN TEST STARTED\r\n");
  printf("=======================\r\n");

  // Set CAN filters
  can.setFilter(targetCANIDFilter, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  while (true) {
    // Report CAN diagnostics
    uint32_t numStreamedMsgsReceived = can.getNumStreamedMsgsReceived();
    uint32_t numOneShotMsgsReceived  = can.getNumOneShotMsgsReceived();
    uint32_t numStreamedMsgsSent     = can.getNumStreamedMsgsSent();
    uint32_t numOneShotMsgsSent      = can.getNumOneShotMsgsSent();

    can.updateStreamedSignal(targetReportDiagnosticsCANID, targetReportNumStreamedSignal, numStreamedMsgsReceived);
    can.updateStreamedSignal(targetReportDiagnosticsCANID, targetReportNumOneShotsSignal, numOneShotMsgsReceived);

    printf("Number of streamed CAN messages received: %lu\r\n", numStreamedMsgsReceived);
    printf("Number of one-shot CAN messages received: %lu\r\n", numOneShotMsgsReceived);
    printf("==========\r\n");
    printf("Number of streamed CAN messages sent: %lu\r\n", numStreamedMsgsSent);
    printf("Number of one-shot CAN messages sent: %lu\r\n\r\n\r\n", numOneShotMsgsSent);

    ThisThread::sleep_for(1ms);
  }
}

static mbed_error_status_t oneShotHandler(CANMsg& msg) {
  return MBED_SUCCESS;
}
