#pragma once

#include "mbed.h"

class Encoder {
 public:
  typedef enum encoderType { relative, absolute } t_encoderType;

  typedef enum quadratureEncodingType { x2_encoding, x4_encoding } t_quadratureEncodingType;

  typedef struct {
    // PWM encoder pins
    PinName pin_PWM = NC;

    // SPI encoder pins
    PinName pin_MOSI = NC;
    PinName pin_MISO = NC;
    PinName pin_SCK  = NC;

    // Quadrature encoder pins
    PinName pin_ChannelA = NC;
    PinName pin_ChannelB = NC;
    PinName pin_Index    = NC;

    float degreesPerUnit     = 1.0;  // If encoder is inverted, set to negative value
    float zeroOffset_Degrees = 0.0;  // Offset after inversion

    // Encoding type (for quadrature encoders)
    t_quadratureEncodingType quadratureEncodingType = x2_encoding;

  } t_encoderConfig;

  Encoder(t_encoderConfig encoderConfig){};
  virtual ~Encoder(){};

  // Must be implemented
  virtual t_encoderType getType()           = 0;
  virtual float getAngle_Degrees()          = 0;
  virtual float getVelocity_DegreesPerSec() = 0;

  // Optionally implemented
  virtual float getRevolutions() {
    return 0.0;
  };
  virtual mbed_error_status_t reset() {
    return 0.0;
  };
};
