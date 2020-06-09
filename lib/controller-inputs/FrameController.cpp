#include "FrameController.h"

using namespace FrameProtocol;

FrameController::FrameController(RawSerial* dest) : m_dest(dest) {
  for (unsigned i = 0; i < MAX_NUM_REDIRECT_FRAME; i++) {
    m_sendSerialThreads[i] = make_unique<Thread>(osPriorityNormal, SERIAL_THREAD_STACK_SIZE, m_sendSerialThreadMems[i]);
  }
}

FrameController::~FrameController() {}

void FrameController::sendFrame(DigitalFrameConfig& frameConfig) {
  static uint8_t m_digitalFrame[FRAME_SIZE_DIGITAL] = {0};

  m_digitalFrame[0]     = SOF_DIGITAL;
  uint8_t second_nibble = 0;

  switch (frameConfig.boardType) {
    case BoardType::MASTER:
      second_nibble |= MASTER_BOARD;
      break;

    case BoardType::ARM:
      second_nibble |= ARM_BOARD;
      break;

    case BoardType::SCIENCE:
      second_nibble |= SCIENCE_BOARD;
      break;

    default:
      second_nibble |= MASTER_BOARD;
      break;
  }

  switch (frameConfig.inputType) {
    case DigitalInputType::BTN:
      second_nibble |= BTN_INPUT;
      break;

    case DigitalInputType::SWITCH:
      second_nibble |= SWITCH_INPUT;
      break;

    default:
      second_nibble |= BTN_INPUT;
      break;
  }

  m_digitalFrame[0] |= (second_nibble << 4);

  uint16_t invalid_bits = 0, digital_values = 0;

  invalid_bits = frameConfig.inputGroup.getValuesAndInvalidReads(digital_values);

  *(uint16_t*)(&m_digitalFrame[1]) = digital_values;
  *(uint16_t*)(&m_digitalFrame[3]) = invalid_bits;

  // The serial port to send data out is a shared resource
  m_mutex.lock();
  for (unsigned int i = 0; i < FRAME_SIZE_DIGITAL; i++) m_dest->putc(m_digitalFrame[i]);
  m_mutex.unlock();
}

void FrameController::sendFrame(AnalogFrameConfig& frameConfig) {
  static uint8_t m_analogFrame[FRAME_SIZE_ANALOG] = {0};

  static uint16_t analog_values[AnalogInputGroup::MAX_INPUTS_NUM] = {0};

  m_analogFrame[0]      = SOF_ANALOG;
  uint8_t second_nibble = 0;

  switch (frameConfig.boardType) {
    case BoardType::MASTER:
      second_nibble |= MASTER_BOARD;
      break;

    case BoardType::ARM:
      second_nibble |= ARM_BOARD;
      break;

    case BoardType::SCIENCE:
      second_nibble |= SCIENCE_BOARD;
      break;

    default:
      second_nibble |= MASTER_BOARD;
      break;
  }

  switch (frameConfig.inputType) {
    case AnalogInputType::JOY:
      second_nibble |= JOY_INPUT;
      break;

    case AnalogInputType::POT:
      second_nibble |= POT_INPUT;
      break;

    default:
      second_nibble |= JOY_INPUT;
      break;
  }

  m_analogFrame[0] = (second_nibble << 4);

  frameConfig.inputGroup.getValues(analog_values);

  for (unsigned i = 0; i < frameConfig.inputGroup.getNumInputs(); i++) {
    *(uint16_t*)(&m_analogFrame[1]) = analog_values[i];

    // The serial port to send data out is a shared resource
    m_mutex.lock();
    for (unsigned int i = 0; i < FRAME_SIZE_ANALOG; i++) m_dest->putc(m_analogFrame[i]);
    m_mutex.unlock();
  }
}

/*
        How redirection works:
        for each source serial, we register a callback on the serial RX IRQ, this callback simply
        puts the char read from the serial port to a queue.
        Meanwhile, we also start a thread, which has access to the same queue used in the callback.
        This thread simply gets element from the queue and accumlates these element to form a frame.
        When the frame is filled, we send the frame out in a crtical section (since the sendFrame function
        could also be called at the same time).

        The important assumption is that the queue is large enough, else we could be losing data
*/
void FrameController::redirectFrame(RawSerial* source) {
  uint8_t i = m_numRegistredFrameRedirections;

  // fail if we try to redirect too much frame
  MBED_ASSERT(i <= MAX_NUM_REDIRECT_FRAME);

  // lazy initialization until we need to register the callback
  m_serialReadCallbacks[i].init(source, &m_serialQueues[i], &m_serialQueueMPools[i]);
  source->attach(callback(&m_serialReadCallbacks[i], &SerialReadCallback::callback), SerialBase::RxIrq);

  // lazy initialization until we need to start the thread
  m_serialSendThreadObjs[i].init(m_dest, &m_mutex, &m_serialQueues[i], &m_serialQueueMPools[i]);
  osStatus status = m_sendSerialThreads[i]->start(callback(&m_serialSendThreadObjs[i], &SerialSendThread::thread));

  // check thread started correctly
  // needs testing ... we might be running out of memory
  MBED_ASSERT(status == osOK);

  m_numRegistredFrameRedirections++;
}

void SerialReadCallback::init(RawSerial* source, Queue<serialQueue_t, SERIAL_QUEUE_SIZE>* queue,
                              MemoryPool<serialQueue_t, SERIAL_QUEUE_SIZE>* mPool) {
  m_source = source;
  m_queue  = queue;
  m_mPool  = mPool;
}

void SerialReadCallback::callback() {
  serialQueue_t* message = m_mPool->alloc();
  *message               = m_source->getc();
  // 0 timeout for enqueue, because having a wait in an IRQ is asking for trouble ...
  m_queue->put(message, 0);
}

void SerialSendThread::init(RawSerial* dest, Mutex* mutex, Queue<serialQueue_t, SERIAL_QUEUE_SIZE>* queue,
                            MemoryPool<serialQueue_t, SERIAL_QUEUE_SIZE>* mPool) {
  m_dest  = dest;
  m_mutex = mutex;
  m_queue = queue;
  m_mPool = mPool;
}

void SerialSendThread::thread() {
  static uint8_t frame[FRAME_SIZE_MAX] = {0};

  uint8_t val = 0, frame_idx = 0, frame_len = 0;
  bool frame_started = false;

  while (1) {
    osEvent evt = m_queue->get(osWaitForever);
    if (evt.status == osEventMessage) {
      serialQueue_t* message = (serialQueue_t*)evt.value.p;
      val                    = *message;
      m_mPool->free(message);
    }

    // if frame has not started, then the byte received would be the first byte in the frame, i.e. SOF
    // we can determine which kind of frame it is based on the SOF
    if (!frame_started) {
      if (val == SOF_DIGITAL) {
        frame_started = true;
        frame_len     = FRAME_SIZE_DIGITAL;

      } else if (val == SOF_ANALOG) {
        frame_started = true;
        frame_len     = FRAME_SIZE_ANALOG;
      }

      frame[frame_idx] = val;
    } else {
      // else keep filling the frame until we have received enough bytes for the frame
      if (frame_idx < frame_len) frame[frame_idx] = val;
    }

    frame_idx++;

    // frame is full, reset parameters, and send out frame
    if (frame_idx == frame_len) {
      frame_idx     = 0;
      frame_started = false;

      // The serial port to send data out is a shared resource
      m_mutex->lock();
      for (unsigned int i = 0; i < frame_len; i++) m_dest->putc(frame[i]);
      m_mutex->unlock();
    }
  }
}
