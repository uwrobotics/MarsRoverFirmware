#include "CANMsg.h"
#include "mbed.h"

CAN can(CAN_RX, CAN_TX);
CANMsg rxMsg;
CANMsg txMsg;
DigitalOut ledTX(LED1);
DigitalOut ledRX(LED2);
Timer timer;
uint8_t counter = 0;

#define TX_ID 0x100
#define RX_ID 0x101

/**
 * @brief   Prints CAN msg to PC's serial terminal
 * @note
 * @param   CANMessage to print
 * @retval
 */
void printMsg(CANMessage& msg) {
  printf("  ID      = 0x%.3x\r\n", msg.id);
  printf("  Type    = %d\r\n", msg.type);
  printf("  Format  = %d\r\n", msg.format);
  printf("  Length  = %d\r\n", msg.len);
  printf("  Data    =");
  for (int i = 0; i < msg.len; i++) printf(" 0x%.2X", msg.data[i]);
  printf("\r\n");
}

int main(void) {
  can.frequency(500000);  // set bit rate to 50kbps
  ledTX = 0;              // set transmit LED off
  ledRX = 0;              // set recieve LED off
  timer.start();          // start timer
  printf("CAN_Hello\r\n");

  while (1) {
    if (timer.elapsed_time() >= 1s) {  // check for timeout
      timer.reset();                // reset timer
      counter++;                    // increment counter
      txMsg.clear();                // clear Tx message storage    // set ID
      txMsg << counter;             // copy counter value to CAN msg payload
      if (can.write(txMsg)) {
        txMsg.id = TX_ID;           // transmit message
        printf("-------------------------------------\r\n");
        printf("CAN message sent\r\n");
        printMsg(txMsg);
        printf("  counter = %d\r\n", counter);
        ledTX = !ledTX;
      } else
        printf("Transmission error\r\n");
    }

    if (can.read(rxMsg)) {
      ledRX = !ledRX;  // turn the LED on
      printf("-------------------------------------\r\n");
      printf("CAN message received\r\n");
      printMsg(rxMsg);

      // Filtering performed by software:
      if (rxMsg.id == RX_ID) {
        rxMsg >> counter;  // extract data from the received CAN message
        printf("  counter = %d\r\n", counter);
        timer.start();  // transmission lag
      }
    }
  }
}
