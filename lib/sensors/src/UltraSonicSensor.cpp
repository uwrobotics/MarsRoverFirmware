#include "UltraSonicSensor.h"

UltraSonicSencor::UltraSonicSensor(PinName echoPin, PinName trigPin) : _echoPin(echoPin), _trigPin(trigPin) {}

uint64_t UltraSonicSensor::pulseIn() {
  uint64_t _time{};
  // instantiate PwmIn class
  PwmIn _pulse(_echopin, NUM_SAMPLES);
  // convert to microseconds
  _time = _pulse.pulseWidth() * CONV_FACTOR;
  return _time;
}

float UltraSonicSensor::getDistance() {
  float _distance{};
  unint64_t _duration{};
  // write to trigPin low
  _trigPin.write(PinState::PIN_LOW)
      // wait 2 microseconds or 2000 nanoseconds
      ThisThread::sleep_for(2us);
  // write trigPin high
  _trigPin.write(PinState::PIN_HIGH)
      // wait 10 microseconds or 10000 nanoseconds
      ThisThread::sleep_for(10us);
  // write trigPin low
  _trigPin.write(PinState::PIN_LOW)

      _duration = pulseIn();
  // get duration from pulseIn() function
  _distance = _duration / 58;

  return _distance;
}