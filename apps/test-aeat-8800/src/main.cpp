#include "mbed.h"

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);  // mosi, miso, sclk (PA_7, PA_6, PA_5)
DigitalOut cs(SPI_CS);                 // cs (PB_6)

int main() {
  uint16_t upper, lower = 0x0000;

  // Select SSI protocol by setting CS high
  cs = 1;
  ThisThread::sleep_for(3ms);

  // Setup the spi for 8 bit data, high steady state clock,
  // second edge capture, with a 1MHz clock rate
  spi.format(16, 3);
  spi.frequency(1000000);

  while (true) {
    // Read
    upper = spi.write(0x8000);
    lower = spi.write(0x0000);

    upper = (upper << 1) | (lower >> 15);
    // lower = (lower << 1) & 0xF000;

    printf("Data = 0x%04X %d \t Diag = 0x%04X\t%d\r\n", upper, upper, lower, lower);
  }
}
