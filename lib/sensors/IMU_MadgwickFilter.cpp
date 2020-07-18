#include "IMU_MadgwickFilter.h"

// ***** Madgwick filter class implementation *****

MadgwickFilter::MadgwickFilter(float updateFreq = DEFAULT_UPDATE_FREQ, float beta = DEFAULT_BETA,
                               float zeta = DEFAULT_ZETA)
    : updateFreq(updateFreq), beta(beta), zeta(zeta), qEst(1.0f, 0.0f, 0.0f, 0.0f) {}

void MadgwickFilter::update(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz) {
  // put IMU measurements in 4D vectors
  vec4f gyro(0, gx, gy, gz);
  vec4f accel(0, ax, ay, az);
  vec4f mag(0, mx, my, mz);

  // angular rate quaternion
  Quaternion qDot = (qEst * gyro) * 0.5f;

  // for clarity and easy access
  float q1 = qEst[0];
  float q2 = qEst[1];
  float q3 = qEst[2];
  float q4 = qEst[3];

  // only compute feedback if accelerometer values are valid (avoid NaN in accelerometer normalization)
  if (ax != 0.0f || ay != 0.0f || az != 0.0f) {
    // normalize accelerometer measurement
    accel.normalize();

    // using accelerometer measurement:

    // objective function: fg(q, a) = q*g*q' - a (where g = [0,0,0,1])
    float fg_x = 2 * (q2 * q4 - q1 * q3) - accel[1];
    float fg_y = 2 * (q1 * q2 + q3 * q4) - accel[2];
    float fg_z = 2 * (0.5f - q2 * q2 - q3 * q3) - accel[3];

    // jacobian: Jg(q)
    float jg_11 = -2 * q3;
    float jg_12 = 2 * q4;
    float jg_13 = -2 * q1;
    float jg_14 = 2 * q2;
    float jg_21 = 2 * q2;
    float jg_22 = 2 * q1;
    float jg_23 = 2 * q4;
    float jg_24 = 2 * q3;
    float jg_31 = 0.0f;
    float jg_32 = -4 * q2;
    float jg_33 = -4 * q3;
    float jg_34 = 0.0f;

    // gradient descent step:

    // function gradient = Jg(q)fg(q, a)
    Quaternion gradient(jg_11 * fg_x + jg_21 * fg_y + jg_31 * fg_z, jg_12 * fg_x + jg_22 * fg_y + jg_32 * fg_z,
                        jg_13 * fg_x + jg_23 * fg_y + jg_33 * fg_z, jg_14 * fg_x + jg_24 * fg_y + jg_34 * fg_z);

    // using magnetometer measurement:

    // only use magnetometer measurement if it is valid (avoid Nan in magnetometer normalization)
    if (mx != 0.0f || my != 0.0f || mz == 0.0f) {
      // normalize magnetometer measurement
      mag.normalize();

      // measured direction of Earth's magnetic field in earth frame
      vec4f h = mag * qEst;

      // to compensate for magnetic distortions
      vec4f b(0, sqrt(h[1] * h[1] + h[2] * h[2]), 0, h[3]);

      // objective function: fb(q, b, m) = q*b*q' - m
      float fb_x = 2 * b[1] * (0.5f - q3 * q3 - q4 * q4) + 2 * b[3] * (q2 * q4 - q1 * q3) - mag[1];
      float fb_y = 2 * b[1] * (q2 * q3 - q1 * q4) + 2 * b[3] * (q1 * q2 + q3 * q4) - mag[2];
      float fb_z = 2 * b[1] * (q1 * q3 + q2 * q4) + 2 * b[3] * (0.5f - q2 * q2 - q3 * q3) - mag[3];

      // jacobian: J(q, b)
      float jb_11 = -2 * b[3] * q3;
      float jb_12 = 2 * b[3] * q4;
      float jb_13 = -4 * b[1] * q3 - 2 * b[3] * q1;
      float jb_14 = -4 * b[1] * q4 + 2 * b[3] * q2;
      float jb_21 = -2 * b[1] * q4 + 2 * b[3] * q2;
      float jb_22 = 2 * b[1] * q3 + 2 * b[3] * q1;
      float jb_23 = 2 * b[1] * q2 + 2 * b[3] * q4;
      float jb_24 = -2 * b[1] * q1 + 2 * b[3] * q3;
      float jb_31 = 2 * b[1] * q3;
      float jb_32 = 2 * b[1] * q4 - 4 * b[3] * q2;
      float jb_33 = 2 * b[1] * q1 - 4 * b[3] * q3;
      float jb_34 = 2 * b[1] * q2;

      // gradient descent step:

      /*
       * combining accelerometer and magnetometer parts:
       * fgb(q, a, b, m) = [fg(q, a), fb(q, b, m)]
       * Jgb(q, b) = [Jg(q), Jb(q, b)]
       */

      // function gradient = Jgb(q)fgb(q, a, b, m)
      // add to previously created function gradient
      gradient += Quaternion(jb_11 * fb_x + jb_21 * fb_y + jb_31 * fb_z, jb_12 * fb_x + jb_22 * fb_y + jb_32 * fb_z,
                             jb_13 * fb_x + jb_23 * fb_y + jb_33 * fb_z, jb_14 * fb_x + jb_24 * fb_y + jb_34 * fb_z);
    }

    gradient.normalize();

#if defined(GYRO_BIAS_DRIFT_COMPENSATION)
    // gyro error = 2 * qEst' * gradient
    vec4f gyroErr = (qEst.conjugated() * gradient) * 2;

    // integrate
    vec4f gyroBias = (gyroErr * (1.0f / updateFreq)) * zeta;

    // compute compensated gyro measurement
    gyro -= gyroBias;

    // recompute angular rate quaternion
    qDot = (qEst * gyro) * 0.5f;
#endif

    // apply feedback
    qDot -= gradient * beta;
  }

  // integrate rate of change and add to current quaternion
  qEst += qDot * (1.0f / updateFreq);
  qEst.normalize();
}

// ***** 4D vector class implementation *****

vec4f::vec4f(float v1, float v2, float v3, float v4) : v1(v1), v2(v2), v3(v3), v4(v4) {}

float& vec4f::operator[](int index) {
  switch (index) {
    case 0:
      return v1;
      break;
    case 1:
      return v2;
      break;
    case 2:
      return v3;
      break;
    case 3:
      return v4;
      break;
    default:
      throw std::out_of_range("Vector index out of range");
      break;
  }
}

vec4f vec4f::operator*(Quaternion q) {
  // temp variables
  float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];
  float q1q1 = q1 * q1;
  float q2q2 = q2 * q2;
  float q3q3 = q3 * q3;
  float q4q4 = q4 * q4;
  float q1q2 = q1 * q2;
  float q3q4 = q3 * q4;

  return vec4f(v1 * (q1q1 + q2q2 + q3q3 + q4q4),
               2 * q1 * (q2 * v1 + q3 * v4 - q4 * v3) + v2 * (q1q1 - q2q2 - q3q3 - q4q4),
               2 * (v2 * (q1 * q4 + q2 * q3) + v4 * (q3q4 - q1q2)) + v3 * (q1q1 - q2q2 + q3q3 - q4q4),
               2 * (v2 * (q2 * q4 - q1 * q3) + v3 * (q1q2 + q3q4)) + v4 * (q1q1 - q2q2 - q3q3 + q4q4));
}

vec4f vec4f::operator-=(vec4f v) {
  v1 -= v[0];
  v2 -= v[1];
  v3 -= v[2];
  v4 -= v[3];
}

void vec4f::normalize(void) {
  float recipNorm = invSqrt(v1 * v1 + v2 * v2 + v3 * v3 + v4 * v4);
  v1 *= recipNorm;
  v2 *= recipNorm;
  v3 *= recipNorm;
  v4 *= recipNorm;
}

// ***** Quaternion class implementation *****

Quaternion::Quaternion(float q1, float q2, float q3, float q4) : q1(q1), q2(q2), q3(q3), q4(q4) {}

float& Quaternion::operator[](int index) {
  switch (index) {
    case 0:
      return q1;
      break;
    case 1:
      return q2;
      break;
    case 2:
      return q3;
      break;
    case 3:
      return q4;
      break;
    default:
      throw std::out_of_range("Quaternion index out of range");
      break;
  }
}

Quaternion Quaternion::operator*(float scalar) {
  return Quaternion(scalar * q1, scalar * q2, scalar * q3, scalar * q4);
}

// returns q * v as a quaternion, where q is a quaternion and v is a 4D vector
Quaternion Quaternion::operator*(vec4f v) {
  return Quaternion(q1 * v[0] - q2 * v[1] - q3 * v[2] - q4 * v[3], q1 * v[1] + q2 * v[0] + q3 * v[3] - q4 * v[2],
                    q1 * v[2] - q2 * v[3] + q3 * v[0] + q4 * v[1], q1 * v[3] + q2 * v[2] - q3 * v[1] + q4 * v[0]);
}

// returns q * qOther as a vector
vec4f Quaternion::operator*(Quaternion q) {
  return vec4f(q1 * q[0] - q2 * q[1] - q3 * q[2] - q4 * q[3], q1 * q[1] + q2 * q[0] + q3 * q[3] - q4 * q[2],
               q1 * q[2] - q2 * q[3] + q3 * q[0] + q4 * q[1], q1 * q[3] + q2 * q[2] - q3 * q[1] + q4 * q[0]);
}

void Quaternion::operator-=(Quaternion q) {
  q1 -= q[0];
  q2 -= q[1];
  q3 -= q[2];
  q4 -= q[3];
}

void Quaternion::operator+=(Quaternion q) {
  q1 += q[0];
  q2 += q[1];
  q3 += q[2];
  q4 += q[3];
}

Quaternion Quaternion::conjugated(void) {
  return Quaternion(q1, -q2, -q3, -q4);
}

void Quaternion::normalize(void) {
  float recipNorm = invSqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
  q4 *= recipNorm;
}

// fast inverse sqare root
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x) {
  long i;
  float x2, y;

  x2 = x * 0.5f;
  y  = x;
  i  = *(long*)&y;
  i  = 0x5f3759df - (i >> 1);
  y  = *(float*)&i;
  y  = y * (1.5f - (x2 * y * y));

  return y;
}