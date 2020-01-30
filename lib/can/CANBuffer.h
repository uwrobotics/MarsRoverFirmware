#pragma once

#ifndef CANBUFFER_DEFAULT_SIZE
#define CANBUFFER_DEFAULT_SIZE 8
#endif

#include "mbed.h"
#include "CircularBuffer.h"
#include "CANMsg.h"

class CANBuffer : CircularBuffer<CANMessage, CANBUFFER_DEFAULT_SIZE> {

public:

    enum BufferType {
        rx,
        tx
    };

    CANBuffer(CAN * CANInterface, BufferType type = rx);

private:

    CAN * p_CANInterface;
    CANMsg m_CANMsg;

    void rxIrqHandler();

    // Remove push from public scope
    using CircularBuffer::push;

};