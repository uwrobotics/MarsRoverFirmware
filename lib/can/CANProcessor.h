#pragma once

#include "mbed.h"

class CANProcessor {

public:
    CANProcessor(CAN &CANInterface);

    void start();
    void stop();

private:


};