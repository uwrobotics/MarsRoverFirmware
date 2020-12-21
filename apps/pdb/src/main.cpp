#ifdef TODO
#include "CANMsg.h"
#include <cmath>
#include "hw_bridge.h"

UltrasonicSensor frontLeft, frontRight, backLeft, backRight;
LEDMatrix matrix;
AnalogIn railBattery, rail5V, rail17V, rail24V;  // add voltage range (as percentage) to hw bridge also allocate can id for reporting if outside range

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
    if(std::abs((railBattery.read()-3) * 100/3) < some percent defined in hw bridge) {send can msg to Xavier} 
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
