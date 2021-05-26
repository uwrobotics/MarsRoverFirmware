#include "CANConfigArm.h"  // Include the header for the board you want to simulate
#include "Logger.h"

CANInterface can(CANConfig::config);

int main() {
  Utility::logger << "";  // Band-aid fix for logger bug (issue #328)

  printf("\r\n\r\n");
  printf("STRESS CAN TEST STARTED\r\n");
  printf("=======================\r\n");

  // Set CAN filters
  can.setFilter(targetCANIDFilter, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 0);
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 1);

  while (true) {
    // Report CAN diagnostics
    uint32_t numStreamedMsgsReceived = can.getNumStreamedMsgsReceived();
    uint32_t numOneShotMsgsReceived  = can.getNumOneShotMsgsReceived();
    uint32_t numStreamedMsgsSent     = can.getNumStreamedMsgsSent();
    uint32_t numOneShotMsgsSent      = can.getNumOneShotMsgsSent();

    can.setTXSignalValue(targetReportDiagnosticsCANID, targetReportNumStreamedSignal, numStreamedMsgsReceived);
    can.setTXSignalValue(targetReportDiagnosticsCANID, targetReportNumOneShotsSignal, numOneShotMsgsReceived);

    printf("Streamed CAN messages received: %lu\r\n", numStreamedMsgsReceived);
    printf("One-shot CAN messages received: %lu\r\n", numOneShotMsgsReceived);
    printf("==========\r\n");
    printf("Streamed CAN messages sent: %lu\r\n", numStreamedMsgsSent);
    printf("One-shot CAN messages sent: %lu\r\n\r\n\r\n", numOneShotMsgsSent);

    // Report CAN faults
    uint16_t numCANRXFaults = can.getNumCANRXFaults();
    uint16_t numCANTXFaults = can.getNumCANTXFaults();

    can.setTXSignalValue(targetReportFaultsCANID, targetNumCANRXFaultsSignal, numCANRXFaults);
    can.setTXSignalValue(targetReportFaultsCANID, targetNumCANTXFaultsSignal, numCANTXFaults);

    printf("CAN RX faults: %u\r\n", numCANRXFaults);
    printf("CAN TX faults: %u\r\n\r\n", numCANTXFaults);

    ThisThread::sleep_for(100ms);
  }
}

static mbed_error_status_t oneShotHandler(void) {
  return MBED_SUCCESS;
}

static mbed_error_status_t switchCANBus(void) {
  HWBRIDGE::CANSignalValue_t canBusID;

  bool success =
      can.getRXSignalValue(HWBRIDGE::CANID::COMMON_SWITCH_CAN_BUS, HWBRIDGE::CANSIGNAL::COMMON_CAN_BUS_ID, canBusID) &&
      can.switchCANBus((HWBRIDGE::CANBUSID)canBusID);

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}
