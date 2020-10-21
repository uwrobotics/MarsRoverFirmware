#pragma once

#include "Pwmln.h"

// declare constants
constexpr int NUM_SAMPLES{1};
constexpr int CONV_FACTOR_FOR_SECONDS_TO_MICROSECONDS{1000000};
constexpr int CONV_FACTOR_FOR_PULSEWISTH_TO_DISTANCE{58};

class UltrasonicSensor {
 public:
  // Constructor for UltrasonicSensor
  UltrasonicSensor(PinName echoPin, PinName trigPin);
  float getDistance();
  uint64_t readPulseWidth();

 private:
  InterruptIn _echoPin;
  DigitalOut _trigPin;
  // instantiate PwmIn pin to be used to get pulsewidth
  PwmIn _pulse(_echopin, NUM_SAMPLES);
  // enum class to contain states of echoPin
  class enum PinState { PIN_LOW = 0, PIN_HIGH = 1 };
};