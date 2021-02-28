#pragma once

#include <stdint.h>

#include "CANMsg.h"
#include "CircularBuffer.h"
#include "mbed.h"

#ifndef CANBUFFER_SIZE
#define CANBUFFER_SIZE 8
#endif

#define CANBUFFER_FLAG_DATA_READY (1UL << 0)
#define CANBUFFER_FLAG_FULL       (1UL << 1)

class CANBuffer : public CircularBuffer<CANMsg, CANBUFFER_SIZE> {
 public:
  enum BufferType { rx, tx };

  CANBuffer(CAN &CANInterface, BufferType type = rx);

  /** Pop the transaction from the buffer
   *
   * @param data Data to be popped from the buffer
   * @return True if the buffer is not empty and data contains a transaction, false otherwise
   */
  bool pop(CANMsg &canMSG);

  uint32_t getFlags();

  uint32_t waitFlagsAny(uint32_t flags = 0, uint32_t millisec = osWaitForever, bool clear = true);

  uint32_t waitFlagsAll(uint32_t flags = 0, uint32_t millisec = osWaitForever, bool clear = true);

 private:
  CAN &r_CANInterface;
  CANMsg m_CANMsg;

  EventFlags m_eventFlags;

  void rxIrqHandler();

  // Remove push from public scope
  using CircularBuffer::push;
};
