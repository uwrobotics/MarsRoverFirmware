#include "CANBuffer.h"

CANBuffer::CANBuffer(CAN &CANInterface, BufferType type): r_CANInterface(CANInterface) {

    if (type == rx) {
        r_CANInterface.attach(callback(this, &CANBuffer::rxIrqHandler), CAN::RxIrq);
    }

}

void CANBuffer::rxIrqHandler(void) {

    if (r_CANInterface.read(m_CANMsg)) {

        m_CANMsg.clear();

        if (full()) {
            // Should do something if full, set a flag to report error etc.
        }

        push(m_CANMsg);
    }
}