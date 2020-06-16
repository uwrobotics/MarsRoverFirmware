#ifndef _IMU_MADGWICK_FILTER_H_
#define _IMU_MADGWICK_FILTER_H_

/*
 * An implementation of the Madgwick AHRS filter.
 * Fuses IMU measurements to produce orientation quaternions.
 * Source: https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
 */

#include <stdexcept>
#include <math.h>


float invSqrt(float x);

class vec4f {
 public:
  float v1, v2, v3, v4;

  vec4f(float v1, float v2, float v3, float v4);

  float& operator[](int index);
  vec4f operator*(float scalar);

  void normalize(void);
  vec4f operator*(Quaternion q);  // returns q * v * q' (where q' is the conjugate of q)
};

class Quaternion {
 public:
  float q1, q2, q3, q4;

  Quaternion(float q1, float q2, float q3, float q4);

  float& operator[](int index);
  Quaternion operator*(float scalar);
  Quaternion operator*(vec4f v);
  void operator-=(Quaternion q);
  void operator+=(Quaternion q);

  Quaternion conjugate(void);
  Quaternion normalized(void);
};

class MadgwickFilter {
 public:
  float updateFreq;  // frequency at which the filter updates its estimate
  float beta;        // divergence rate of gyro-derived orientation
  Quaternion qEst;   // orientation estimate

  MadgwickFilter(float beta, float updateFreq);

  // (ax, ay, az) - accelerometer measurement
  // (gx, gy, gz) - gyro measurement
  // (mx, my, mz) - magnetometer measurement
  void update(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
};

#endif