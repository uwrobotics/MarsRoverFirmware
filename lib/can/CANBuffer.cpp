#include "CANBuffer.h"

CANBuffer::CANBuffer(CAN * CANInterface, BufferType type): p_CANInterface(CANInterface) {

    if (type == rx) {
        p_CANInterface->attach(callback(this, &CANBuffer::rxIrqHandler), CAN::RxIrq);
    }

}

void CANBuffer::rxIrqHandler(void) {

    if (p_CANInterface->read(m_CANMsg)) {

        m_CANMsg.clear();

        if (!full()) {
            push(m_CANMsg);
        }
    }
}