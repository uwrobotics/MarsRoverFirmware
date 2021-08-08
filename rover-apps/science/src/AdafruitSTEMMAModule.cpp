#include <ratio>

#include "AdafruitSTEMMAModule.h"
#include "Module.h"
#include "mbed.h"

AdafruitSTEMMAModule::AdafruitSTEMMAModule(PinName sda, PinName scl) : moistureSensor(sda, scl) {
  static int numFails = 0;
  bool resetStatus    = false;

  while (!resetStatus && numFails < 10) {
    resetStatus = moistureSensor.reset();
  }
  // if still error then log through printf (currently), plan to later log through LOGGER (TODO)
  printf("Cannot Reset Moisture Sensor");
}

void AdafruitSTEMMAModule::periodic_1s(void) {}
void AdafruitSTEMMAModule::periodic_10s(void) {}
void AdafruitSTEMMAModule::periodic_10ms(void) {}
void AdafruitSTEMMAModule::periodic_1ms(void) {}

void AdafruitSTEMMAModule::periodic_100ms(void) {
  static int numFails = 0;
  bool updateStatus   = false;

  while (!updateStatus && numFails < 10) {
    updateStatus = moistureSensor.update();
  }
  // if still error then log through printf (currently), plan to later log through LOGGER (TODO)
}
