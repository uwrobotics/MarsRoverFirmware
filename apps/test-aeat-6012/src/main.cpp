#include "AEAT6012.h"
#include "mbed.h"

constexpr auto ENCODER_READ_PERIOD = 500ms;

enum Test_Mode_E { TEST_BLOCKING, TEST_ASYNC };

void callback(void);
void test_blocking(void);
void test_async(void);

Timer timer;
volatile bool encoder_position_updated = false;

Encoder::AEAT6012 encoder(SPI_CS, SPI_MISO, SPI_SCK);

// Modify this
Test_Mode_E test_mode = TEST_BLOCKING;

int main() {
  switch (test_mode) {
    case TEST_BLOCKING:
      test_blocking();
      break;

    case TEST_ASYNC:
      test_async();
      break;

    default:
      break;
  }
}

void test_blocking(void) {
  printf("\r\n--- AEAT-6012 Blocking Driver Test ---\r\n\r\n");

  while (true) {
    timer.reset();
    timer.start();

    encoder.read();

    timer.stop();

    printf("Encoder position degrees: %.3f\r\n", encoder.get_position_deg());
    printf("Encoder position raw: %u\r\n", encoder.get_position_raw());
    printf("Encoder angular velocity deg/s: %.3f\r\n", encoder.get_angular_velocity_dps());

    printf("Read time: %llu us\r\n\r\n",
           std::chrono::duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count());

    ThisThread::sleep_for(ENCODER_READ_PERIOD);
  }
}

void test_async(void) {
  printf("\r\n--- AEAT-6012 Async Driver Test ---\r\n\r\n");

  while (true) {
    timer.reset();
    timer.start();

    while (!encoder.read_async(callback)) {
    }

    while (!encoder_position_updated) {
    }

    timer.stop();

    encoder_position_updated = false;

    printf("Encoder reading degrees: %.3f\r\n", encoder.get_position_deg());
    printf("Encoder reading raw: %u\r\n", encoder.get_position_raw());
    printf("Encoder angular velocity deg/s: %.3f\r\n", encoder.get_angular_velocity_dps());

    printf("Read time: %llu us\r\n\r\n",
           std::chrono::duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count());

    ThisThread::sleep_for(ENCODER_READ_PERIOD);
  }
}

void callback(void) {
  encoder_position_updated = true;
}
