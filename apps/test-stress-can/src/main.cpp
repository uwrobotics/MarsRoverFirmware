#include "mbed.h"
#include "CANBus.h"

CANBus can(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

DigitalOut led(LED1);
InterruptIn btn(BUTTON_1);

constexpr uint16_t sent = 1000;
uint16_t received = 0;
 
// create an event queue
EventQueue queue;
 
void receiveMessage() {
  received++;
}
 
int main() {
  // create a thread that'll run the event queue's dispatch function
  Thread eventThread;
  eventThread.start(callback(&queue, &EventQueue::dispatch_forever));
 
  // wrap calls in queue.event to automatically defer to the queue's thread
  btn.fall(queue.event(&receiveMessage));
 
  for(;;);
}
