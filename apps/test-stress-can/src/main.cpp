#include "CANBus.h"
#include "CANMsg.h"
#include "mbed.h"

CANBus can(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
 
// create an event queue
EventQueue queue;

void CANReceiveEventHandler(CANMsg &msg) {

  static uint16_t received = 0;


  // Print data received from the CAN msg 
  printf("  Data    =");
  for (int i = 0; i < msg.len; i++) printf(" 0x%.2X", msg.data[i]);
  printf("\r\n");

  received++;
}
 
void CANMsgIRQHandler() {
  CANMsg msg;
  if (can.read(msg)) {
    queue.call(&CANReceiveEventHandler, msg);

  } else {
    printf("THIS SHOULD NEVER HAPPEN \r\n");
    MBED_ASSERT(false);
  }
}

int main() {
  // create a thread that'll run the event queue's dispatch function
  Thread rxCANProcessorThread(osPriorityRealtime);
  rxCANProcessorThread.start(callback(&queue, &EventQueue::dispatch_forever));

  // wrap calls in queue.event to automatically defer to the queue's thread
  can.attach(&CANMsgIRQHandler, CANBus::RxIrq);

  while (true)
    ;

}
