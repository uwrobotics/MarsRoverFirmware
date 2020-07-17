#pragma once

#include <memory>

#include "AnalogInputGroup.h"
#include "DigitalInputGroup.h"
#include "mbed.h"

/*
  When sending controller board data to the PC, we will use a custom protocol with two different
  frames for digital and analog inputs respectively.
  Digital inputs frame (one frame per digital inputs group):
    SOF(4 bits, 0b1010, i.e. 0xA),
    Board Selection: 2 bits, because 3 subboards,
    Control element type selection: 2 bits, because 2 type of controls, pushbutton and switch,
                                    the other bit is for future expansion,
    Control element selection/value: 2 bytes, each bit corresponds to the value of the control element),
    Control element value valid: 2 bytes, each bit corresponds to whether the value is valid or not)
  Total: 5 bytes

  Analog input frame (one frame per analog input):
    SOF(4 bits, 0b0101, i.e. 0x5),
    Board Selection: 2 bits, because 3 subboards,
    Control element type selection: 2 bits, because 2 type of controls, joystick and potentiometer,
                                    the other bit is for future expansion,
    Control element value : 2 bytes, because each analog value is a uint16_t, note that analog values are always valid
  Total: 3 bytes

  EOF is not necesarry, since we can know the start of frame, and the number of bytes in a frame is fixed.
  The two types of frames are of fixed size, for easier parsing on the receving end
*/

namespace FrameProtocol {

enum class BoardType { MASTER, ARM, SCIENCE };
enum class DigitalInputType { BTN, SWITCH };
enum class AnalogInputType { JOY, POT };

// some consts/defines needed by multiple classes
static constexpr uint8_t FRAME_SIZE_DIGITAL = 5, FRAME_SIZE_ANALOG = 3;
static constexpr uint8_t FRAME_SIZE_MAX =
    (FRAME_SIZE_DIGITAL > FRAME_SIZE_ANALOG) ? FRAME_SIZE_DIGITAL : FRAME_SIZE_ANALOG;
static constexpr uint8_t SOF_DIGITAL = 0xA, SOF_ANALOG = 0x5;
static constexpr uint8_t SERIAL_QUEUE_SIZE = 50;
typedef uint8_t serialQueue_t;

/*
        This class encapsulates a callback to be attached to a Serial object
*/
class SerialReadCallback {
 private:
  /*
     use a pointer instead of reference, because reference must be initialized in constructor
     but we need lazy initialization
     we should not free the pointer in destructor because they serve as references
  */
  RawSerial* m_source;
  Queue<serialQueue_t, SERIAL_QUEUE_SIZE>* m_queue;
  MemoryPool<serialQueue_t, SERIAL_QUEUE_SIZE>* m_mPool;

 public:
  SerialReadCallback(){};

  void init(RawSerial* source, Queue<serialQueue_t, SERIAL_QUEUE_SIZE>* queue,
            MemoryPool<serialQueue_t, SERIAL_QUEUE_SIZE>* mPool);

  void callback();
};

/*
        This class is used to start a thread that sends serial data
*/
class SerialSendThread {
 private:
  RawSerial* m_dest;
  Mutex* m_mutex;
  Queue<serialQueue_t, SERIAL_QUEUE_SIZE>* m_queue;
  MemoryPool<serialQueue_t, SERIAL_QUEUE_SIZE>* m_mPool;

 public:
  SerialSendThread(){};

  void init(RawSerial* m_dest, Mutex* mutex, Queue<serialQueue_t, SERIAL_QUEUE_SIZE>* queue,
            MemoryPool<serialQueue_t, SERIAL_QUEUE_SIZE>* mPool);

  void thread();
};

/*
        The class that encapsulates our custom protocol
*/
class FrameController {
 public:
  struct DigitalFrameConfig {
    BoardType boardType;
    DigitalInputType inputType;
    DigitalInputGroup& inputGroup;
  };

  struct AnalogFrameConfig {
    BoardType boardType;
    AnalogInputType inputType;
    AnalogInputGroup& inputGroup;
  };

  FrameController(RawSerial* dest);

  ~FrameController();

  void sendFrame(DigitalFrameConfig& frameConfig);

  void sendFrame(AnalogFrameConfig& frameConfig);

  void redirectFrame(RawSerial* source);

 private:
  static constexpr uint8_t MASTER_BOARD = 0x0, ARM_BOARD = 0x1, SCIENCE_BOARD = 0x2;
  static constexpr uint8_t BTN_INPUT = 0x0 << 2, SWITCH_INPUT = 0x1 << 2, JOY_INPUT = 0x0 << 2, POT_INPUT = 0x1 << 2;
  static constexpr uint8_t MAX_NUM_REDIRECT_FRAME    = 2;
  static constexpr uint16_t SERIAL_THREAD_STACK_SIZE = 300;

  RawSerial* m_dest;

  Mutex m_mutex;

  uint8_t m_numRegistredFrameRedirections = 0;

  Queue<serialQueue_t, SERIAL_QUEUE_SIZE> m_serialQueues[MAX_NUM_REDIRECT_FRAME];
  MemoryPool<serialQueue_t, SERIAL_QUEUE_SIZE> m_serialQueueMPools[MAX_NUM_REDIRECT_FRAME];

  // no need to initialize the thread stack mem
  uint8_t m_sendSerialThreadMems[MAX_NUM_REDIRECT_FRAME][SERIAL_THREAD_STACK_SIZE];

  // had to dynamically allocate ... since we can't initialize its fields properly if allocated statically
  std::unique_ptr<Thread> m_sendSerialThreads[MAX_NUM_REDIRECT_FRAME];
  SerialSendThread m_serialSendThreadObjs[MAX_NUM_REDIRECT_FRAME] = {};

  SerialReadCallback m_serialReadCallbacks[MAX_NUM_REDIRECT_FRAME] = {};
};

}  // namespace FrameProtocol