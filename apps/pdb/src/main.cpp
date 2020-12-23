#ifdef TODO
#include <cmath>

#include "CANMsg.h"
#include "hw_bridge.h"

UltrasonicSensor frontLeft(ULTRASONIC_RX, ULTRASONIC_TX, SOME_SLAVE_ADDRESS),
    frontRight(ULTRASONIC_RX, ULTRASONIC_TX, SOME_SLAVE_ADDRESS),
    backLeft(ULTRASONIC_RX, ULTRASONIC_TX, SOME_SLAVE_ADDRESS),
    backRight(ULTRASONIC_RX, ULTRASONIC_TX, SOME_SLAVE_ADDRESS);
LEDMatrix matrix(LED_MATRIX_R_CHANNEL, LED_MATRIX_G_CHANNEL, LED_MATRIX_B_CHANNEL);
AnalogIn railBattery(RAIL_BATTERY_ANLG_IN), rail5V(RAIL_5V_ANLG_IN), rail17V(RAIL_17V_ANLG_IN),
    rail24V(RAIL_24V_ANLG_IN);  // add voltage range (as percentage) to hw bridge also allocate can id for reporting if
                                // outside range

CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

void rxCANProcessor();
void txCANProcessor();

void setLEDMatrix();  // simple switch statement that calls a different function based on contents of CAN msg

const static CANMsg::CANMsgHandlerMap canHandlerMap = {
    {HWBRIDGE::CANID::NEOPIXEL_SET, &setLEDMatrix}};  // rename NEOPIXEL_SET to LEDMATRIX_SET

int main() {
  Thread rxCANProcessorThread(osPriorityAboveNormal);
  Thread txCANProcessorThread(osPriorityBelowNormal);
  while (true) {
    if (std::abs((railBattery.read() - 3) * 100 / 3) < some percent defined in hw bridge) {
      send can msg to Xavier
    }
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
#else
#define BLINKING_RATE 500ms
int main() {
  // Initialise the digital pin LED1 as an output
  DigitalOut led(LED1);

  while (true) {
    led = !led;
    printf("Current LED State is %s\n", led ? "ON" : "OFF");
    ThisThread::sleep_for(BLINKING_RATE);
  }
}
#endif
