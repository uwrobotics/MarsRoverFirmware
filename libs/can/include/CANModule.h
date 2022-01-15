#pragma onceusing 

#include "Module.h"
#include "mbed.h"
#include "CANInterface.h"

class CANModule final : public Module {
    public:

    CANModule();

    void periodic_1ms(void) override {}
    void periodic_10ms(void) override {}


    private:
    CANInterface 
}