#include "ActuatorController.h"
#include "CANMsg.h"
#include "hw_bridge.h"
#include "mbed.h"
namespace PIDTuningApi{
    static mbed_error_status_t setPIDParameter(const CANMsg &msg);
    static bool allowPIDTuning = false;
    ActuatorController testActuator();
}