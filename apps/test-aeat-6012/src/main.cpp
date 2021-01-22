#include "AEAT6012.h"
#include "mbed.h"

#define ENCODER_READ_FREQUENCY_HZ 1
#define ENCODER_READ_PERIOD       (1000ms / (ENCODER_READ_FREQUENCY_HZ))

void callback(int event);
void read_encoder(void);

Thread read_encoder_thread;
volatile bool encoder_position_updated = false;

AEAT6012::AEAT6012 encoder(SPI_CS, SPI_MOSI, SPI_SCK, callback);

int main() {
  read_encoder_thread.start(read_encoder);
  while (true) {
    if (encoder_position_updated) {
      printf("Encoder reading: %.3f degrees\r\n", encoder.get_position());
      encoder_position_updated = false;
    }
  }
}

void callback(int event) {
  encoder_position_updated = true;
}

void read_encoder(void) {
  encoder.read_position_async();
  ThisThread::sleep_for(ENCODER_READ_PERIOD);
}
