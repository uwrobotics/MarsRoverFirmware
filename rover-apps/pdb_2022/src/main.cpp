#include <cmath>

#include "CANBus.h"
#include "CANMsg.h"
#include "Logger.h"
#include "hw_bridge.h"

AnalogIn railBattery(RAIL_BATTERY_ANLG_IN), rail5V(RAIL_5V_ANLG_IN), rail17V(RAIL_17V_ANLG_IN),
    rail24V(RAIL_24V_ANLG_IN);  // add voltage range (as percentage) to hw bridge also allocate can id for reporting if
                                // outside range

CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVER_CANBUS_FREQUENCY_HZ);

void rxCANProcessor();
void txCANProcessor();

// simple switch statement that calls a different function based on contents of CAN msg
static mbed_error_status_t setLEDMatrix(void);

const static CANMsg::CANMsgHandlerMap canHandlerMap = {{HWBRIDGE::CANID::PDB_SET_LED_MATRIX, &setLEDMatrix}};

int main() {
  Thread rxCANProcessorThread(osPriorityAboveNormal);
  Thread txCANProcessorThread(osPriorityBelowNormal);
  while (true) {
    // if (std::abs((railBattery.read() - 3) * 100 / 3) < some percent defined in hw bridge) {send to xavier}
    // repeat for each digital in
  }
}

void rxCANProcessor() {
  CANMsg rxMsg;
  while (true) {
    if (can1.read(rxMsg)) {
      canHandlerMap.at(rxMsg.getID())();
    }
    ThisThread::sleep_for(2ms);
  }
}

void txCANProcessor() {
  CANMsg txMsg;
  while (true) {
    // stream ultrasonic data
    ThisThread::sleep_for(2ms);  // TODO: define the sleep rate in the bridge
  }
}

static mbed_error_status_t setLEDMatrix(void) {
  return MBED_ERROR_INVALID_ARGUMENT;
}
