#include "NetzerEncoder.h"
#include "mbed.h"


#define ENCODER_READ_FREQUENCY_HZ 1
#define ENCODER_READ_PERIOD       (1000ms / (ENCODER_READ_FREQUENCY_HZ))

void callback(void);
void run_async();
Timer timer;
volatile bool encoder_position_updated = false;
NetzerEncoder::NetzerEncoder encoder(SPI_MISO, SPI_MOSI, SPI_SCK, callback);

int main(){
	run_async();
}


void run_async() {
  printf("\r\n--- Netzer Encoder Async Driver Test App ---\r\n");

  while (true) {
    timer.reset();
    timer.start();

    while (!encoder.spi_read()) {
    }

    while (!encoder_position_updated) {
    }

    timer.stop();

    encoder_position_updated = false;

    printf("Encoder reading raw: %d\r\n", encoder.get_raw_data());

    printf("Read time: %llu us\r\n",
           std::chrono::duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count());

    ThisThread::sleep_for(ENCODER_READ_PERIOD);
  }
}

void callback(void) {
  encoder_position_updated = true;
}