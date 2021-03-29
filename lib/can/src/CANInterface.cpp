#include "CANInterface.h"

CANInterface::CANInterface(const Config &config)
    : m_CANBus1(config.can1_RX, config.can1_TX, config.frequency_hz),
      m_CANBus2(config.can2_RX, config.can2_TX, config.frequency_hz),
      m_activeCANBus(&m_CANBus1),
      m_rxPostmanThread(RX_POSTMAN_THREAD_PRIORITY),
      m_rxClientThread(RX_CLIENT_THREAD_PRIORITY),
      m_txProcessorThread(TX_PROCESSOR_THREAD_PRIORITY),
      m_rxStreamedMsgMap(config.rxStreamedMsgMap),
      m_txStreamedMsgMap(config.txStreamedMsgMap),
      m_rxOneShotMsgHandler(config.rxOneShotMsgHandler) {
  // Set hw filters
  m_CANBus1.setFilter(config.filterID, config.filterFormat, config.filterMask);
  m_CANBus2.setFilter(config.filterID, config.filterFormat, config.filterMask);

  // Put CAN bus 2 in silent monitoring mode
  m_CANBus2.monitor(true);

  // Processing threads
  m_rxPostmanThread.start(callback(&m_rxEventQueue, &EventQueue::dispatch_forever));
  m_rxClientThread.start(callback(this, &CANInterface::rxClient));
  m_txProcessorThread.start(callback(this, &CANInterface::txProcessor));

  // RX IRQ
  m_CANBus1.attach(callback(this, &CANInterface::rxISR), CAN::RxIrq);
  m_CANBus2.attach(callback(this, &CANInterface::rxISR), CAN::RxIrq);
}

void CANInterface::rxISR(void) {
  can_irq_set(m_activeCANBus->getHandle(), IRQ_RX, false);
  m_rxEventQueue.call(callback(this, &CANInterface::rxPostman));
}

void CANInterface::rxPostman(void) {
  CANMsg msg;
  // this loop is needed to avoid missing msg received between turning off the IRQ and turning it back on
  while (m_activeCANBus->read(msg)) {
    CANMsg *mail = m_rxMailbox.try_alloc();  // no wait

    if (mail != nullptr) {
      *mail = msg;
      MBED_ASSERT(m_rxMailbox.put(mail) == osOK);
    } else {
      MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_BUFFER_FULL), "CAN RX mailbox full");
    }
  }
  can_irq_set(m_activeCANBus->getHandle(), IRQ_RX, true);
}

void CANInterface::rxClient(void) {
  while (true) {
    CANMsg *mail = nullptr;

    do {
      mail = m_rxMailbox.try_get();  // TODO: try_get_for was not working. Investigate why and use it
      ThisThread::sleep_for(1ms);
    } while (mail == nullptr);

    // Extract message
    CANMsg msg = *mail;
    MBED_ASSERT(m_rxMailbox.free(mail) == osOK);

    // If message is streamed, extract message signals and put in RX message map
    if (m_rxStreamedMsgMap->contains(msg.getID())) {
      HWBRIDGE::CANMsgData_t msgData;
      msg.getPayload(msgData);

      if (!HWBRIDGE::unpackCANMsg(msgData.raw, msg.getID(), m_rxStreamedMsgMap)) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                     "CAN RX message unpacking failed");
      }
    }

    // Otherwise if message is one-shot, process message
    else if (m_rxOneShotMsgHandler->at(msg.getID())(msg) == MBED_SUCCESS) {
      // Don't need to do anything here
    }

    // Otherwise invalid message was received
    else {
      MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                   "Invalid CAN message received");
    }
  }
}

void CANInterface::txProcessor(void) {
  while (true) {
    auto startTime = Kernel::Clock::now();

    CANMsg *mail = nullptr;

    // Send all one-shots messages that were queued
    while ((mail = m_txMailboxOneShot.try_get()) != nullptr) {
      if (!m_activeCANBus->write(*mail)) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED), "CAN TX write failed");
      }
      MBED_ASSERT(m_txMailboxOneShot.free(mail) == osOK);
      ThisThread::sleep_for(TX_INTERDELAY);
    }

    // Send all streamed messages
    for (auto it = m_txStreamedMsgMap->begin(); it != m_txStreamedMsgMap->end(); it++) {
      HWBRIDGE::CANID msgID          = it->first;
      HWBRIDGE::CANMsgData_t msgData = {0};
      size_t len                     = 0;

      if (HWBRIDGE::packCANMsg(msgData.raw, msgID, m_txStreamedMsgMap, len)) {
        // Send message
        CANMsg msg;
        msg.setID(msgID);
        msg.setPayload(msgData, len);
        m_activeCANBus->write(msg);
        ThisThread::sleep_for(TX_INTERDELAY);
      } else {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                     "CAN RX message packing failed");
      }
    }

    ThisThread::sleep_until(startTime + TX_PERIOD);
  }
}

bool CANInterface::sendOneShotMessage(CANMsg &msg) {
  CANMsg *mail = m_txMailboxOneShot.try_alloc_for(1ms);
  if (mail) {
    *mail = msg;
    MBED_ASSERT(m_txMailboxOneShot.put(mail) == osOK);
    return true;
  }
  return false;
}

bool CANInterface::updateStreamedSignal(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                                        HWBRIDGE::CANSignalValue_t signalValue) {
  return m_txStreamedMsgMap->setSignalValue(msgID, signalName, signalValue);
}

bool CANInterface::readStreamedSignal(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                                      HWBRIDGE::CANSignalValue_t &signalValue) {
  return m_rxStreamedMsgMap->getSignalValue(msgID, signalName, signalValue);
}

void CANInterface::switchCANBus(HWBRIDGE::CANBUSID canBusID) {
  switch (canBusID) {
    case HWBRIDGE::CANBUSID::CANBUS1:
      m_activeCANBus = &m_CANBus1;
      m_CANBus1.monitor(false);
      m_CANBus2.monitor(true);
      break;

    case HWBRIDGE::CANBUSID::CANBUS2:
      m_activeCANBus = &m_CANBus2;
      m_CANBus1.monitor(true);
      m_CANBus2.monitor(false);
      break;

    default:
      break;
  }
}
