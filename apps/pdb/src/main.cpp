#include <cmath>

#include "CANBus.h"
#include "CANMsg.h"
#include "hw_bridge.h"

AnalogIn railBattery(RAIL_BATTERY_ANLG_IN), rail5V(RAIL_5V_ANLG_IN), rail17V(RAIL_17V_ANLG_IN),
    rail24V(RAIL_24V_ANLG_IN);  // add voltage range (as percentage) to hw bridge also allocate can id for reporting if
                                // outside range

CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

void rxCANProcessor();
void txCANProcessor();

static mbed_error_status_t setLEDMatrix(
    CANMsg &msg);  // simple switch statement that calls a different function based on contents of CAN msg

const static CANMsg::CANMsgHandlerMap canHandlerMap = {
    {HWBRIDGE::CANID::NEOPIXEL_SET, &setLEDMatrix}};  // rename NEOPIXEL_SET to LEDMATRIX_SET

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
      canHandlerMap.at(rxMsg.getID())(rxMsg);
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

static mbed_error_status_t setLEDMatrix(CANMsg &msg) {
  return MBED_ERROR_INVALID_ARGUMENT;
}
