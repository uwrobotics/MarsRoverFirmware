#include "PIDTuningApi.h"

static mbed_error_status_t PIDTuningApi::setPIDParameter(const CANMsg& msg) {
    printf("Received request to update PID params over CAN");
    if(!PIDTuningApi::allowPIDParamTuning) {
        printf("Unable to update PID params over CAN. Ensure arm is in a safe state and update Tuning Mode.\n");
        printf("To allow PID param tuning over CAN, send true to CAN address SET_PID_TUNING_MODE\n");
        return MBED_ERROR_ASSERTION_FAILED;
    }
    HWBRIDGE::ARM::PID::tuningApiPayload payload;
    msg.getPayload(payload);
    printf("payload: %s", HWBRIDGE::ARM::PID::str(payload).c_str());
}