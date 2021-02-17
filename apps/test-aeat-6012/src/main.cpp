#include "AEAT6012.h"
#include "Logger.h"
#include "mbed.h"

constexpr auto ENCODER_READ_PERIOD = 1ms;
constexpr auto PRINTF_PERIOD       = 500ms;

enum Test_Mode_E { TEST_BLOCKING, TEST_ASYNC };

void encoder_read_blocking(void);
void encoder_read_async(void);
void callback(void);

Thread thread;
Timer timer;

volatile bool encoder_position_updated         = false;
volatile float encoder_angle_deg               = 0;
volatile float encoder_angular_vel_deg_per_sec = 0;
volatile uint32_t encoder_pos_read_time_us     = 0;
volatile uint32_t encoder_vel_read_time_us     = 0;

Encoder::AEAT6012 encoder(SPI_MISO, SPI_SCK, SPI_CS);

// Modify this
Test_Mode_E test_mode = TEST_BLOCKING;

int main() {
  switch (test_mode) {
    case TEST_BLOCKING:
      thread.start(encoder_read_blocking);
      break;

    case TEST_ASYNC:
      thread.start(encoder_read_async);
      break;

    default:
      break;
  }

  while (true) {
    Utility::Logger::printf("Encoder position degrees: %.3f\n", encoder_angle_deg);
    Utility::Logger::printf("Read time: %lu us\n\n", encoder_pos_read_time_us);

    Utility::Logger::printf("Encoder angular velocity deg/s: %.3f\n", encoder_angular_vel_deg_per_sec);
    Utility::Logger::printf("Read time: %lu us\n\n", encoder_vel_read_time_us);

    ThisThread::sleep_for(PRINTF_PERIOD);
  }
}

void encoder_read_blocking(void) {
  Utility::Logger::printf("\n--- AEAT-6012 Blocking Driver Test ---\n\n");
  float measurement;
  encoder.reset();

  while (true) {
    timer.reset();
    timer.start();

    encoder.getAngleDeg(measurement);

    timer.stop();

    encoder_angle_deg        = measurement;
    encoder_pos_read_time_us = std::chrono::duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count();

    ThisThread::sleep_for(1ms);

    timer.reset();
    timer.start();

    encoder.getAngularVelocityDegPerSec(measurement);

    timer.stop();

    encoder_angular_vel_deg_per_sec = measurement;
    encoder_vel_read_time_us = std::chrono::duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count();

    ThisThread::sleep_for(ENCODER_READ_PERIOD);
  }
}

void encoder_read_async(void) {
  Utility::Logger::printf("\n--- AEAT-6012 Async Driver Test ---\n\n");
  encoder.reset();

  while (true) {
    timer.reset();
    timer.start();

    uint16_t attempts = 5;
    while (attempts > 0 && !encoder.readAsync(callback)) {
      attempts--;
    }

    if (attempts == 0) {
      Utility::Logger::printf("ENCODER READ FAILED AFTER TOO MANY RETRIES\n");
    } else {
      // Wait for callback
      while (!encoder_position_updated)
        ;

      timer.stop();

      encoder_position_updated = false;

      encoder_angle_deg               = encoder.getAngleDegNoTrigger();
      encoder_angular_vel_deg_per_sec = encoder.getAngularVelocityDegPerSecNoTrigger();
      encoder_pos_read_time_us        = encoder_vel_read_time_us =
          std::chrono::duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count();
    }

    ThisThread::sleep_for(ENCODER_READ_PERIOD);
  }
}

void callback(void) {
  encoder_position_updated = true;
}
