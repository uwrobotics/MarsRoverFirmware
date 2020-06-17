#ifndef _IMU_MADGWICK_FILTER_H_
#define _IMU_MADGWICK_FILTER_H_

/*
 * An implementation of the Madgwick AHRS filter.
 * Fuses IMU measurements to produce orientation quaternions.
 * Source: https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
 */

#include <math.h>

#include <stdexcept>

#ifndef GYRO_BIAS_DRIFT_COMPENSATION
#define GYRO_BIAS_DRIFT_COMPENSATION  // define to apply gyro bias drift compensation in filter
#endif

float invSqrt(float x);

class vec4f {
 public:
  float v1, v2, v3, v4;

  vec4f(float v1, float v2, float v3, float v4);

  float& operator[](int index);
  vec4f operator*(float scalar);
  vec4f operator-=(vec4f v);

  void normalize(void);
  vec4f operator*(Quaternion q);  // returns q * v * q' (where q' is the conjugate of q)
                                  // represents v rotated by q
};

class Quaternion {
 public:
  float q1, q2, q3, q4;

  Quaternion(float q1, float q2, float q3, float q4);

  float& operator[](int index);
  Quaternion operator*(float scalar);
  Quaternion operator*(vec4f v);  // returns q * v as a quaternion
  vec4f operator*(Quaternion q); // return q * qOther as a vector
  void operator-=(Quaternion q);
  void operator+=(Quaternion q);

  Quaternion conjugated(void);
  void normalize(void);
};

class MadgwickFilter {
 public:
  float updateFreq;  // frequency at which the filter updates its estimate
  float beta;        // represents mean zero gyro measurement error
  float zeta;        // gain used for gyro bias drift compensation
  Quaternion qEst;   // orientation estimate

  MadgwickFilter(float updateFreq, float beta, float zeta = 0.0f);

  /*
   * update() should be continuously called at the update frequency
   *
   * (ax, ay, az) - accelerometer measurement
   * (gx, gy, gz) - gyro measurement
   * (mx, my, mz) - magnetometer measurement
   */
  void update(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
};

#endif