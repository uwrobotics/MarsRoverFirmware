#pragma once

#include "Module.h"
#include "mbed.h"
#include "CANInterface.h"

class ControllerModule final : public Module {
   
    public:
        ControllerModule(CANInterface &can);

    void periodic_1s(void) override;
    void periodic_10s(void) override;
    void periodic_100ms(void) override;
    void periodic_10ms(void) override;
    void periodic_1ms(void) override;

    private:
        CANInterface* m_can;
}