/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin), m_servoRangeInDegrees(servoRangeInDegrees), m_minPulsewidthInMs(minPulsewidthInMs), m_maxPulsewidthInMs(maxPulsewidthInMs) {}

void TutorialServo::setAngleRangeInDegrees(float degrees) {
    m_servoRangeInDegrees = degrees;
}

void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs) {
    m_minPulsewidthInMs = minPulsewidthMs;
    m_maxPulsewidthInMs = maxPulsewidthMs;
}

void TutorialServo::setPositionInDegrees(float degrees) {
    m_servoPwmOut.pulsewidth(m_minPulsewidthInMs/1000 + (degrees/m_servoRangeInDegrees)*((m_maxPulsewidthInMs-m_minPulsewidthInMs)/1000));
}

int main(){
    return 0;
}


