#include "CANBus.h"
#include "CANMsg.h"
#include "Logger.h"
#include "hw_bridge.h"
#include "mbed.h"

CANBus can(CAN_RX, CAN_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
CANMsg rxMsg;
CANMsg txMsg;
DigitalOut ledTX(LED1);
DigitalOut ledRX(LED2);
Timer timer;
uint8_t counter = 0;

constexpr uint16_t TX_ID = 0x100;
constexpr uint16_t RX_ID = 0x101;

/**
 * @brief   Prints CAN msg to PC's serial terminal
 * @note
 * @param   CANMsg to print
 * @retval
 */
void printMsg(CANMsg& msg) {
  Utility::Logger::printf("  ID      = 0x%.3x\r\n", static_cast<uint16_t>(msg.getID()));
  Utility::Logger::printf("  Type    = %d\r\n", msg.type);
  Utility::Logger::printf("  Format  = %d\r\n", msg.format);
  Utility::Logger::printf("  Length  = %d\r\n", msg.len);
  Utility::Logger::printf("  Data    =");
  for (int i = 0; i < msg.len; i++) Utility::Logger::printf(" 0x%.2X", msg.data[i]);
  Utility::Logger::printf("\r\n");
}

int main(void) {
  ledTX = 0;      // set transmit LED off
  ledRX = 0;      // set recieve LED off
  timer.start();  // start timer
  Utility::Logger::printf("CAN_Hello\r\n");

  while (1) {
    if (timer.elapsed_time() >= 1s) {                    // check for timeout
      timer.reset();                                     // reset timer
      counter++;                                         // increment counter
      txMsg.clear();                                     // clear Tx message storage
      txMsg.setID(static_cast<HWBRIDGE::CANID>(TX_ID));  // set ID
      txMsg << counter;                                  // copy counter value to CAN msg payload
      if (can.write(txMsg)) {                            // transmit message
        Utility::Logger::printf("-------------------------------------\r\n");
        Utility::Logger::printf("CAN message sent\r\n");
        printMsg(txMsg);
        Utility::Logger::printf("  counter = %d\r\n", counter);
        ledTX = !ledTX;
      } else
        Utility::Logger::printf("Transmission error\r\n");
    }

    if (can.read(rxMsg)) {
      ledRX = !ledRX;  // turn the LED on
      Utility::Logger::printf("-------------------------------------\r\n");
      Utility::Logger::printf("CAN message received\r\n");
      printMsg(rxMsg);

      // Filtering performed by software:
      if (rxMsg.getID() == static_cast<HWBRIDGE::CANID>(RX_ID)) {
        rxMsg >> counter;  // extract data from the received CAN message
        Utility::Logger::printf("  counter = %d\r\n", counter);
        timer.start();  // transmission lag
      }
    }
  }
}
