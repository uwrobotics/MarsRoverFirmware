#pragma once

#include "Pwmln.h"
#include "mbed.h"

// declare constants
const int NUM_SAMPLES{1};
const int CONV_FACTOR{1000000};

class UltraSonicSensor {
 public:
  // Constructor for UltrasonicSensor
  UltraSonicSensor(PinName echoPin, PinName trigPin);
  float getDistance();
  uint64_t pulseIn();

 private:
  InterruptIn _echoPin;
  DigitalOut _trigPin;
  // enum class to contain states of echoPin
  class enum PinState { PIN_LOW = 0, PIN_HIGH = 1 };
};