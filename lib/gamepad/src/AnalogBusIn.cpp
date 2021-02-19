#include "AnalogBusIn.h"

AnalogBusIn::AnalogBusIn(PinName p0, PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6,
                         PinName p7) {
  PinName pins[MAX_INPUTS_NUM] = {p0, p1, p2, p3, p4, p5, p6, p7};

  // No lock needed in the constructor
  _nc_mask = 0;
  for (int i = 0; i < MAX_INPUTS_NUM; i++) {
    _pin[i] = (pins[i] != NC) ? std::make_unique<AnalogIn>(pins[i]) : nullptr;
    if (pins[i] != NC) {
      _nc_mask |= (1 << i);
    }
  }
}

AnalogBusIn::AnalogBusIn(PinName pins[MAX_INPUTS_NUM]) {
  // No lock needed in the constructor
  _nc_mask = 0;
  for (int i = 0; i < MAX_INPUTS_NUM; i++) {
    _pin[i] = (pins[i] != NC) ? std::make_unique<AnalogIn>(pins[i]) : nullptr;
    if (pins[i] != NC) {
      _nc_mask |= (1 << i);
    }
  }
}

AnalogBusIn::~AnalogBusIn() {
  // No lock needed in the destructor, and since we use smart pointers, no need for delete
}

void AnalogBusIn::read(float values[MAX_INPUTS_NUM]) {
  lock();
  for (int i = 0; i < MAX_INPUTS_NUM; i++) {
    if (_pin[i] != 0) {
      values[i] = _pin[i]->read();
    }
  }
  unlock();
}

void AnalogBusIn::read_u16(uint16_t values[MAX_INPUTS_NUM]) {
  lock();
  for (int i = 0; i < MAX_INPUTS_NUM; i++) {
    if (_pin[i] != 0) {
      values[i] = _pin[i]->read_u16();
    }
  }
  unlock();
}

void AnalogBusIn::lock() {
  _mutex.lock();
}

void AnalogBusIn::unlock() {
  _mutex.unlock();
}

AnalogIn &AnalogBusIn::operator[](int index) {
  // No lock needed since _pin is not modified outside the constructor
  MBED_ASSERT(index >= 0 && index < MAX_INPUTS_NUM);
  MBED_ASSERT(_pin[index]);
  return *_pin[index];
}
