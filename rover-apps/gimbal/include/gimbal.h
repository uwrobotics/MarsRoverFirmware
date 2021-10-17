#pragma once

#include "Module.h"
#include "mbed.h"
#include "LimServo.h"
/*This PDB module is for load, rail and temperature monitoring.*/

class gimbal final : public Module {
 public:
  /* Sets the Load DIAG_EN pins */
  void periodic_1s(void) override {}
  void periodic_10s(void) override {}
  void periodic_100ms(void) override {}
  void periodic_10ms(void) override;
  void periodic_1ms(void) override {}

 private:
  void recieve_can_message();
  Actuator::LimServo pitchServo(SRVO_PWM_PITCH, Internal::RANGE, Internal::MAX_PULSE, Internal::MIN_PULSE);

};

