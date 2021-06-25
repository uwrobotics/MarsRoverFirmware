#pragma once

class Module {
 public:
  virtual void periodic_10s(void)   = 0;
  virtual void periodic_1s(void)    = 0;
  virtual void periodic_100ms(void) = 0;
  virtual void periodic_10ms(void)  = 0;
  virtual void periodic_1ms(void)   = 0;
};
