#ifdef TODO
#include "CANMsg.h"
#include "hw_bridge.h"

UltrasonicSensor frontLeft, frontRight, backLeft, backRight;
LEDMatrix matrix;
DigitalIn rail;  // add voltage range to led matrix also allocate can id for reporting if outside range

CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

void rxCANProcessor();
void txCANProcessor();

void setLEDMatrix(); // simple switch statement that calls a different function based on contents of CAN msg

const static CANMsg::CANMsgHandlerMap canHandlerMap = {
    {HWBRIDGE::CANID::NEOPIXEL_SET, &setLEDMatrix}};  // rename NEOPIXEL_SET to LEDMATRIX_SET

int main() {
  Thread rxCANProcessorThread(osPriorityAboveNormal);
  Thread txCANProcessorThread(osPriorityBelowNormal);
  while (true) {
    // if(rail outside range) {send can msg to Xavier} need to define 
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
    ThisThread::sleep_for(2ms); // TODO: define the sleep rate in the bridge
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

