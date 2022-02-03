#pragma onceusing

#include "CANInterface.h"
#include "Module.h"
#include "mbed.h"

class CANModule final : public Module {
 public:
  CANModule(const Config &config);

  void periodic_1ms(void) override {}
  void periodic_10ms(void) override {}

 private:
  CANInterface interface;

};