#include "CANInterface.h"

CANInterface::CANInterface(const Config &config)
    : m_CANBus1(config.can1_RX, config.can1_TX, config.frequency_hz),
      m_CANBus2(config.can2_RX, config.can2_TX, config.frequency_hz),
      m_activeCANBus(&m_CANBus1),
      m_rxPostmanThread(RX_POSTMAN_THREAD_PRIORITY),
      m_rxMsgMap(config.rxMsgMap),
      m_txMsgMap(config.txMsgMap),
      m_rxOneShotMsgHandler(config.rxOneShotMsgHandler),
      m_numStreamedMsgsReceived(0),
      m_numOneShotMsgsReceived(0),
      m_numStreamedMsgsSent(0),
      m_numOneShotMsgsSent(0),
      m_numCANRXFaults(0),
      m_numCANTXFaults(0) {
  // Put CAN bus 2 in silent monitoring mode, also disable CAN 2 RX interrupts
  m_CANBus2.monitor(true);
  can_irq_set(m_CANBus2.getHandle(), IRQ_RX, false);

  // Processing threads
  m_rxPostmanThread.start(callback(&m_rxEventQueue, &EventQueue::dispatch_forever));

  // RX ISR
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
      m_numCANRXFaults++;
    }
  }
  can_irq_set(m_activeCANBus->getHandle(), IRQ_RX, true);
}

void CANInterface::rxClientPeriodic(void) {
  CANMsg *mail = nullptr;
  // Check if a message has arrived:
  mail = m_rxMailbox.try_get(); 

  MBED_ASSERT(mail != nullptr);

  // Extract message
  CANMsg msg = *mail;
  MBED_ASSERT(m_rxMailbox.free(mail) == osOK);

  // Check if message is intended to be received by this node
  m_rxMutex.lock();
  bool validMsgReceived = (m_rxMsgMap != nullptr) && m_rxMsgMap->contains(msg.getID());
  m_rxMutex.unlock();

  if (validMsgReceived) {
    HWBRIDGE::CANMsgData_t msgData;
    msg.getPayload(msgData);

    // Extract message signals and put into RX message map
    m_rxMutex.lock();
    bool msgUnpacked = HWBRIDGE::unpackCANMsg(msgData.raw, msg.getID(), m_rxMsgMap);
    m_rxMutex.unlock();

    if (msgUnpacked) {
      // If message is one-shot, process message
      if ((m_rxOneShotMsgHandler != nullptr) && m_rxOneShotMsgHandler->contains(msg.getID())) {
        if (m_rxOneShotMsgHandler->at(msg.getID())() != MBED_SUCCESS) {
          MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_FAILED_OPERATION),
                        "Failed to process CAN message");
        }
        m_numOneShotMsgsReceived++;
      }
      // Otherwise message is streamed
      else {
        m_numStreamedMsgsReceived++;
      }
    } else {
      MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                    "CAN RX message unpacking failed");
      m_numCANRXFaults++;
    }
  }

  // Otherwise invalid message was received
  else {
    MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                  "Invalid CAN message received");
    m_numCANRXFaults++;
  }
}

void CANInterface::txProcessorPeriodic(void) {

  CANMsg *mail = nullptr;

  // Send all one-shot messages that were queued
  while ((mail = m_txMailboxOneShot.try_get()) != nullptr) {
    if (!m_activeCANBus->write(*mail)) {
      MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED), "CAN TX write failed");
      m_numCANTXFaults++;
    }
    MBED_ASSERT(m_txMailboxOneShot.free(mail) == osOK);
  }

  // Send all streamed messages
  if (m_txMsgMap != nullptr) {
    for (auto it = m_txMsgMap->begin(); it != m_txMsgMap->end(); it++) {
      HWBRIDGE::CANID msgID          = it->first;
      HWBRIDGE::CANMsgData_t msgData = {0};
      size_t len                     = 0;

      m_txMutex.lock();
      bool msgPacked = HWBRIDGE::packCANMsg(msgData.raw, msgID, m_txMsgMap, len);
      m_txMutex.unlock();

      if (msgPacked) {
        // Send message
        CANMsg msg;
        msg.setID(msgID);
        msg.setPayload(msgData, len);
        m_activeCANBus->write(msg);

        m_numStreamedMsgsSent++;

      } else {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                      "CAN TX message packing failed");
        m_numCANTXFaults++;
      }
    }
  }
}

bool CANInterface::sendOneShotMessage(CANMsg &msg, Kernel::Clock::duration_u32 timeout) {
  CANMsg *mail = m_txMailboxOneShot.try_alloc_for(timeout);
  if (mail) {
    *mail = msg;
    MBED_ASSERT(m_txMailboxOneShot.put(mail) == osOK);
    m_numOneShotMsgsSent++;
    return true;
  }
  return false;
}

bool CANInterface::setTXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                                    HWBRIDGE::CANSignalValue_t signalValue) {
  m_txMutex.lock();
  bool success = (m_txMsgMap != nullptr) && m_txMsgMap->setSignalValue(msgID, signalName, signalValue);
  m_txMutex.unlock();
  return success;
}

bool CANInterface::getRXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                                    HWBRIDGE::CANSignalValue_t &signalValue) {
  m_rxMutex.lock();
  bool success = (m_rxMsgMap != nullptr) && m_rxMsgMap->getSignalValue(msgID, signalName, signalValue);
  m_rxMutex.unlock();
  return success;
}

bool CANInterface::switchCANBus(HWBRIDGE::CANBUSID canBusID) {
  bool success = false;

  switch (canBusID) {
    case HWBRIDGE::CANBUSID::CANBUS1:
      if (m_activeCANBus != &m_CANBus1) {
        // Disable CAN 2
        can_irq_set(m_CANBus2.getHandle(), IRQ_RX, false);
        m_CANBus2.monitor(true);

        // Enable CAN 1
        m_activeCANBus = &m_CANBus1;
        can_irq_set(m_CANBus1.getHandle(), IRQ_RX, true);
        m_CANBus1.monitor(false);
      }
      success = true;
      break;

    case HWBRIDGE::CANBUSID::CANBUS2:
      if (m_activeCANBus != &m_CANBus2) {
        // Disable CAN 1
        can_irq_set(m_CANBus1.getHandle(), IRQ_RX, false);
        m_CANBus1.monitor(true);

        // Enable CAN 2
        m_activeCANBus = &m_CANBus2;
        can_irq_set(m_CANBus2.getHandle(), IRQ_RX, true);
        m_CANBus2.monitor(false);
      }
      success = true;
      break;

    default:
      success = false;
      break;
  }

  return success;
}

bool CANInterface::setFilter(HWBRIDGE::CANFILTER filter, CANFormat format, uint16_t mask, int handle) {
  bool success = true;
  if (handle >= 0 && handle < 14) {
    success &= m_CANBus1.setFilter(filter, format, mask, handle);       // CAN 1 filter banks: 0-13
    success &= m_CANBus2.setFilter(filter, format, mask, handle + 14);  // CAN 2 filter banks: 14-27
  }
  return success;
}

uint32_t CANInterface::getNumStreamedMsgsReceived(void) {
  return m_numStreamedMsgsReceived;
}

uint32_t CANInterface::getNumOneShotMsgsReceived(void) {
  return m_numOneShotMsgsReceived;
}

uint32_t CANInterface::getNumStreamedMsgsSent(void) {
  return m_numStreamedMsgsSent;
}

uint32_t CANInterface::getNumOneShotMsgsSent(void) {
  return m_numOneShotMsgsSent;
}

uint16_t CANInterface::getNumCANRXFaults(void) {
  return m_numCANRXFaults;
}

uint16_t CANInterface::getNumCANTXFaults(void) {
  return m_numCANTXFaults;
}
