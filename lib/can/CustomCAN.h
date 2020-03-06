#pragma once

#include "CAN.h"

class CustomCAN : public CAN {

public:

    /** Read a CANMessage from the bus without mutexing.
     *
     *  @param msg A CANMessage to read to.
     *  @param handle message filter handle (0 for any message)
     *
     *  @returns
     *    0 if no message arrived,
     *    1 if message arrived
     */
    int readNonLocking(CANMessage &msg, int handle = 0) {
        return can_read(&_can, &msg, handle);
    }

};