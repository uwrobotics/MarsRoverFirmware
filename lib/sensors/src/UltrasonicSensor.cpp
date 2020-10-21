#include "UltrasonicSensor.h"

UltrasonicSencor::UltrasonicSensor(PinName echoPin, PinName trigPin) : _echoPin(echoPin), _trigPin(trigPin) {}

uint64_t UltrasonicSensor::readPulseWidth() {
  // get pulsewidth in seconds then convert to microseconds
  uint64_t _time = _pulse.pulseWidth() * CONV_FACTOR_FOR_SECONDS_TO_MICROSECONDS;
  return _time;
}

float UltrasonicSensor::getDistance() {
  // write to trigPin low
  _trigPin.write(PinState::PIN_LOW)
      // wait 2 microseconds or 2000 nanoseconds
      ThisThread::sleep_for(2us);
  // write trigPin high
  _trigPin.write(PinState::PIN_HIGH)
      // wait 10 microseconds or 10000 nanoseconds
      ThisThread::sleep_for(10us);
  // write trigPin low
  _trigPin.write(PinState::PIN_LOW);
  unint64_t _duration = readPulseWidth();
  // get duration from pulseIn() function
  float _distance = _duration / CONV_FACTOR_FOR_PULSEWISTH_TO_DISTANCE;

  return _distance;
}