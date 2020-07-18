#include <array>

#include "IMU.h"
#include "mbed.h"

// Pins for SPI - configure these based on MCU pin mapping
#define MOSI SPI1_SCK
#define MISO SPI1_SCK
#define SCLK SPI1_SCK
#define CS   PA_0

#define SPI_FREQ 7000000

// Parameters for IMU orientation estimation (these will need to be tuned)
#define UPDATE_FREQ 512.0f  // frequency at which the madgwick filter updates its estimate
#define BETA        0.1f    // represents mean zero gyro measurement error
#define ZETA        0.0f    // gain used for gyro bias drift compensation

IMU imu(MOSI, MISO, SCLK, CS, UPDATE_FREQ, BETA, ZETA);
Thread thread;

// Measurement data (for ROS messages)
// std::array<double, 3> linear_accel;
// std::array<double, 3> angular_velocity;
// std::array<double, 3> magnetic_field;
// std::array<double, 4> orientation_quaternion;

bool init_imu() {
  imu.init(SPI_FREQ);
  return imu.is_connected();
}

// IMU::update_orientation() needs to be continuously called at the specified update frequency
void orientation_update_thread() {
  while (1) {
    imu.update_orientation();
    ThisThread::sleep_for(1 / UPDATE_FREQ * 1000);
  }
}

// Print AGM (accelerometer, gyroscope, magnetometer) data
void print_scaled_agm(AGM_scaled_t agm) {
  printf("[Accelerometer (m/s^2)]: x = %.2f, y = %.2f, z = %.2f\r\n", agm.acc.x, agm.acc.y, agm.acc.z);
  printf("[Gyroscope (rad/s)]: x = %.2f, y = %.2f, z = %.2f\r\n", agm.gyr.x, agm.gyr.y, agm.gyr.z);
  printf("[Magnetometer (T)]: x = %.2f, y = %.2f, z = %.2f\r\n", agm.mag.x, agm.mag.y, agm.mag.z);
  printf("\r\n");
}

int main() {
  if (!init_imu()) {
    printf("Error setting up IMU\r\n");
    return -1;
  }

  thread.start(orientation_update_thread);

  // Keep polling for measurements
  while (1) {
    if (imu.data_ready()) {
      imu.update_AGM();
      print_scaled_agm(imu.agm_scaled);

      // For ROS messages:
      //   linear_accel           = imu.get_lin_accel();
      //   angular_velocity       = imu.get_ang_vel();
      //   magnetic_field         = imu.get_mag_field();
      //   orientation_quaternion = imu.get_orientation();

      ThisThread::sleep_for(50);
    }
  }

  return 0;
}