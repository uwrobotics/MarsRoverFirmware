#include "CANBuffer.h"

CANBuffer::CANBuffer(CAN &CANInterface, BufferType type): r_CANInterface(CANInterface) {

    if (type == rx) {
        r_CANInterface.attach(callback(this, &CANBuffer::rxIrqHandler), CAN::RxIrq);
    }

}

bool CANBuffer::pop(CANMsg &canMSG) {

    core_util_critical_section_enter();

    bool data_popped = CircularBuffer::pop(canMSG);

    if (empty()) {
        m_eventFlags.clear(CANBUFFER_FLAG_DATA_READY | CANBUFFER_FLAG_FULL);
    } 
    else if (!full()) {
        m_eventFlags.clear(CANBUFFER_FLAG_FULL);
    }

    core_util_critical_section_exit();

    return data_popped;
}

void CANBuffer::rxIrqHandler(void) {

    if (r_CANInterface.readNonLocking(m_CANMsg)) {

        core_util_critical_section_enter();

        m_eventFlags.set(CANBUFFER_FLAG_DATA_READY);
        push(m_CANMsg);

        if (full()) {
            m_eventFlags.set(CANBUFFER_FLAG_FULL);
        }

        core_util_critical_section_exit();
    }
}

uint32_t CANBuffer::getFlags() {
    return m_eventFlags.get();
}

uint32_t CANBuffer::waitFlagsAny(uint32_t flags, uint32_t millisec) {
    return m_eventFlags.wait_all(flags, millisec, false);
}

uint32_t CANBuffer::waitFlagsAll(uint32_t flags, uint32_t millisec) {

    return m_eventFlags.wait_any(flags, millisec, false);
}