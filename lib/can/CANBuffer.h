#pragma once

#include "mbed.h"
#include "CircularBuffer.h"
#include "CANMsg.h"
#include <stdint.h>

#ifndef CANBUFFER_DEFAULT_SIZE
#define CANBUFFER_DEFAULT_SIZE 8
#endif

class CANBuffer : CircularBuffer<CANMsg, CANBUFFER_DEFAULT_SIZE> {

public:

    enum BufferType {
        rx,
        tx
    };

    CANBuffer(CAN &CANInterface, BufferType type = rx);

    /** Pop the transaction from the buffer
     *
     * @param data Data to be popped from the buffer
     * @return True if the buffer is not empty and data contains a transaction, false otherwise
     */
    bool pop(CANMsg &canMSG);

    /** Check if the buffer is empty
     *
     * @return True if the buffer is empty, false if not
     */
    bool empty() const;

    /** Check if the buffer is full
     *
     * @return True if the buffer is full, false if not
     */
    bool full() const;

    /** Reset the buffer
     *
     */
    void reset();

    /** Get the number of elements currently stored in the circular_buffer */
    uint32_t size() const;

    /** Peek into circular buffer without popping
     *
     * @param data Data to be peeked from the buffer
     * @return True if the buffer is not empty and data contains a transaction, false otherwise
     */
    bool peek(CANMsg &canMSG) const;

private:

    CAN &r_CANInterface;
    CANMsg m_CANMsg;

    void rxIrqHandler();

    // Remove push from public scope
    using CircularBuffer::push;

};