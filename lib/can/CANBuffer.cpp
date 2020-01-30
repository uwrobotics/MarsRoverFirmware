#include "CANBuffer.h"

CANBuffer::CANBuffer(CAN * CANInterface, BufferType type): p_CANInterface(CANInterface) {

    if (type == rx) {
        Callback rxCallback(this, &rxIrqHandler);
        p_CANInterface->attach(rxCallback, CAN::RxIrq);
    }

}

CANBuffer::rxIrqHandler() {

    if (can.read(m_CANMsg)) {

        m_CANMsg.clear();

        if (!full()) {
            push(m_CANMsg);
        }
    }
}