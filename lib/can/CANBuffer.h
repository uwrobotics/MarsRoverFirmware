#pragma once

#ifndef CANBUFFER_DEFAULT_SIZE
#define CANBUFFER_DEFAULT_SIZE 8 // 1 kHz
#endif

#include "CAN.h"
#include "CANMsg.h"
#include "CircularBuffer.h"

class CANBuffer : CircularBuffer<CANMessage, CANBUFFER_DEFAULT_SIZE> {

public:

    enum BufferType {
        rx,
        tx
    };

    CANBuffer(CAN * CANInterface, BufferType type = rx);

    bool pop();
    bool empty();
    bool full();
    void reset();
    CounterType size();
    bool peek(CANMsg &msg);

private:

    CAN * p_CANInterface;
    CANMsg m_CANMsg;

    void rxIrqHandler();

    // Remove push from public scope
    using CircularBuffer::push();

};