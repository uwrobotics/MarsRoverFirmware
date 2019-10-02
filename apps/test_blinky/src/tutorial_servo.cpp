//
// Created by Shu Yan on 2019-06-30.
//
#include "mbed.h"
#include "tutorial_servo.h"

void TutorialServo::setRangeInDegrees(float degrees){
    _rangeInDegrees = degrees;
}

void TutorialServo::setPositionInDegrees(float degrees){
    _servoPwmOut.pulsewidth(0.001+ 0.001*degrees/_rangeInDegrees);
}

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegree) : _servoPwmOut(servoPin), _rangeInDegrees(servoRangeInDegree) {_servoPwmOut.period(0.020);}
