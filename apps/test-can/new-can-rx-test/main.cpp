#include <vector>

#include "CANBus.h"
#include "CANMsg.h"

static int received_msgs = 0;

CANBus bus(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

EventQueue queue;

DigitalOut led(LED1);

Mail<CANMsg, 100> mailBox;

void work(CANMsg &msg) {
  received_msgs++;
  led = !led;
  if (msg.id != 0x123 || msg.len != 1 || msg.data[0] != 0xAA) {
    printf("Warning: Got wrong id, msg, or size\r\n");
  }
  // do some work
  std::vector<float> foo(50);
  float bar = 0.0015;
  for (auto &i : foo) {
    i = bar * 3.14 - 40.45212;
    bar++;
  }
}

void consumer() {
  while (true) {
    CANMsg *mail = nullptr;
    do {
      mail = mailBox.try_get();  // TODO: try_get_for was not working. Investigate why and use it
      ThisThread::sleep_for(1ms);
    } while (mail == nullptr);
    MBED_ASSERT((mail != nullptr) && true);
    work(*mail);
    MBED_ASSERT(mailBox.free(mail) == osOK);
  }
}

void postman() {
  CANMsg msg;
  while (bus.read(msg)) {
    CANMsg *mail = mailBox.try_alloc_for(1ms);
    MBED_ASSERT((mail != nullptr) || false);
    *mail = msg;
    MBED_ASSERT(mailBox.put(mail) == osOK);
  }
  can_irq_set(bus.getcan_t(), IRQ_RX, true);
}

void canisr() {
  can_irq_set(bus.getcan_t(), IRQ_RX, false);
  queue.call(&postman);
}

int main() {
  printf("#############################\r\n");
  printf("#### TEST CAN STARTED ####\r\n");
  printf("#############################\r\n");
  Thread rxThread(osPriorityRealtime);
  Thread consumerThread(osPriorityAboveNormal);
  rxThread.start(callback(&queue, &EventQueue::dispatch_forever));
  consumerThread.start(consumer);
  bus.attach(canisr, CANBus::RxIrq);
  Timer timer;
  timer.start();
  ThisThread::sleep_for(10s);
  printf("Received %d/1000 msgs\r\n", received_msgs);
  while (true) {
    ThisThread::sleep_for(1000s);
  }
}
