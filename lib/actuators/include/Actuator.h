#pragma once

/*
 * The Actuator class is an abstracted class that encompasses
 * DC motors, brushless motors, limit servos, and continuous servos.
 * What "value" represents depends on the actuator type:
 *
 *      Actuator Type       Value
 *      ------------------------------
 *      DC Motor            Speed
 *      Brushless Motor     Speed
 *      Limit Servo         Position
 *      Continuous Servo    Speed
 */

namespace Actuator {
class Actuator {
 public:
  virtual void setValue(float value) = 0;
  virtual float getValue() const     = 0;
  virtual ~Actuator() {}
};
}
