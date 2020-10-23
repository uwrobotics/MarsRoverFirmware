#pragma once

#include "mbed.h"

/*
 * The Actuator class is an abstracted class that encompasses
 * DC motors, limit servos, and continuous servos. What "value"
 * represents depends on the actuator type:
 *
 *      Actuator Type       Value
 *      ------------------------------
 *      DC Motor            Speed
 *      Limit Servo         Position
 *      Continuous Servo    Speed
 */

class Actuator {
 public:
  Actuator()          = default;
  virtual ~Actuator() = default;

  virtual void setValue(float value){};

  virtual Actuator& operator=(float value){};

  virtual float getValue() {
    return -1;
  }
};
