#include "mbed.h"

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);  // mosi, miso, sclk (PA_7, PA_6, PA_5)

const int WORDS                = 2;    // Since we want 14 bits, left space for 2, 8 bit words
const uint8_t tx_buffer[WORDS] = {0};  // Initialize dummy tx
uint8_t rx_buffer[WORDS];              // Initialize rx buffer to receive data (16 bits in total)

// Callback function, still need to figure out how to fully implement it
void spi_read(int events) {
  printf("First buffer: %u  Second buffer: %u ", rx_buffer[0], rx_buffer[0]);
}
const event_callback_t spi_read_callback = spi_read;

int main() {
  spi.format(16, 3);  // I think this may have to be 8 bit?
  spi.frequency(500000);

  ThisThread::sleep_for(3ms);

  while (true) {
    spi.transfer(tx_buffer, WORDS, rx_buffer, WORDS, spi_read_callback, NULL);
  }
}
