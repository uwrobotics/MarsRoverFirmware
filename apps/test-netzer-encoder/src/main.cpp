#include "mbed.h"

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);  // mosi, miso, sclk (PA_7, PA_6, PA_5)

int main() {

  uint16_t upper, lower;
  uint16_t ack, start, zero, data, error, warn, crc;

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

    ack   = (upper >> 13) & 1;
    start = (upper >> 12) & 1;
    zero  = (upper >> 11) & 1;

    data = (upper << 5) | (lower >> 11);

    error = (lower >> 10) & 1;
    warn  = (lower >> 9) & 1;

    crc = (lower << 9) >> 11;

    uint32_t msg = crc | (warn << 6) | (error << 7) | (data << 8);
    uint32_t rem = msg % crcPolynomial;

    printf("Upper: 0x%04X \t Lower: 0x%04X \n", upper, lower);
    printf("Ack: %d \t Start: %d \t Zero: %d \t Data: %d \t Error: %d \t Warn: %d \t CRC: %d \t Rem: %d \n\n",
           ack, start, zero, data, error, warn, crc, (int) rem);
  }
}
