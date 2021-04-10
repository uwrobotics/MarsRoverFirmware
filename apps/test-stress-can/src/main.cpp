#include "CANConfigPDB.h"
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
    uint32_t numMsgsReceived = can.getNumMsgsReceived();
    can.updateStreamedSignal(targetReportDiagnosticsCANID, targetReportCANStatsSignal, numMsgsReceived);
    printf("Number of CAN messages received: %lu\r\n", numMsgsReceived);

    ThisThread::sleep_for(1ms);
  }
}

static mbed_error_status_t oneShotHandler(CANMsg& msg) {
  return MBED_SUCCESS;
}
