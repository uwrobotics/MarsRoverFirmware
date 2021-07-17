#pragma once

#include "Module.h"
#include "mbed.h"
#include "AdafruitSTEMMAModule.h"
#include "AdafruitSTEMMA.h"

class AdafruitSTEMMAModule final : public Module {
    public:
        AdafruitSTEMMAModule();

        void periodic_1s(void) override;
        void periodic_10s(void) override {}
        void periodic_100ms(void) override {}
        void periodic_10ms(void) override {}
        void periodic_1ms(void) override {}
    
    private:
        AdafruitSTEMMA moistureSensor;
}
