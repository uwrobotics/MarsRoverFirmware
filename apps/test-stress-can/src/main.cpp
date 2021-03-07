#include "CANBus.h"
#include "CANMsg.h"
#include "mbed.h"

void implementTraditionalMethod();
void implementInterruptMethod();

CANBus can(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// create an event queue
EventQueue queue;

// This variable isn't made locally because the implementInterruptMethod function needs access to it to print the number
// of messages received
uint16_t ISRReceived = 0;

// create a Timer to limit the length that the process can take
Timer timer;

void CANReceiveEventHandler(CANMsg &msg) {
  // Print data received from the CAN msg
  printf("  Data    =");
  for (int i = 0; i < msg.len; i++) printf(" 0x%.2X", msg.data[i]);
  printf("\r\n");

  ISRReceived++;
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
  printf('Start of sequence');
  printf('\r\n');
  implementTraditionalMethod();
  implementInterruptMethod();
}

void implementInterruptMethod() {
  timer.reset();
  timer.start();
  Thread rxCANProcessorThread(osPriorityRealtime);
  rxCANProcessorThread.start(callback(&queue, &EventQueue::dispatch_forever));

  // wrap calls in queue.event to automatically defer to the queue's thread
  can.attach(&CANMsgIRQHandler, CANBus::RxIrq);

  while (true) {
    if (timer.elapsed_time().count() > 30000000) break; // 30 million microseconds, or 30 second limit
  }

  printf("Total Messages Received: " + ISRReceived);
  printf("\r\n");
}

void implementTraditionalMethod() {
  timer.reset();
  timer.start();
  static uint16_t received = 0;
  CANMsg msg;

  // CAN messages are read and printed for a maximum of 30 seconds
  while (timer.elapsed_time().count() < 30000000) {
    if (can.read(msg)) {
      // Print data received from the CAN msg
      printf("  Data    =");
      for (int i = 0; i < msg.len; i++) {
        printf(" 0x%.2X", msg.data[i]);
      }
      for (int i = 0; i < msg.len; i++) printf(" 0x%.2X", msg.data[i]);
      printf("\r\n");

      received++;
    }
  }

  printf("Total Messages Received: " + received);
  printf("\r\n");
}
