#include "mbed.h"

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);  // mosi, miso, sclk (PA_7, PA_6, PA_5)

int main() {

  uint16_t upper, lower;
  uint32_t msg;

  uint16_t ack, start, zero, error, warn, crc;
  uint32_t pos;

  const uint8_t crcPolynomial = 0x03;

  ThisThread::sleep_for(3ms);

  // Setup the spi for 14 bit data, high steady state clock,
  // second edge capture, with a 500 kHz clock rate
  spi.format(16, 3);
  spi.frequency(500000);

  while (true) {
    // Read
    upper = spi.write(0x8000);
    lower = spi.write(0x0000);

    msg   = (((upper << 16) | lower) >> 2) & 0x0FFFFFFF;

    ack   = (msg >> 27) & 1;
    start = (msg >> 26) & 1;
    zero  = (msg >> 25) & 1;

    pos  = (msg >> 8) & 0x1FFFF;

    error = (msg >> 7) & 1; // 1 = good
    warn  = (msg >> 6) & 1; // 1 = good

    crc   = msg & 0x3F;

    uint32_t rem = (msg & 0x1FFFFFF) % crcPolynomial;

    printf("Upper: 0x%04X \t Lower: 0x%04X \n", upper, lower);
    printf("Ack: %d \t Start: %d \t Zero: %d \t Pos: %d \t Error: %d \t Warn: %d \t CRC: %d \t Rem: %d \n",
           ack, start, zero, (int) pos, error, warn, crc, (int) rem);
    printf("Pos: 0x%08X \t CRC: 0x%04X \t Msg: 0x%08X \n\n", (unsigned int) pos, crc, (unsigned int) msg);
  }
}