#include "CANBus.h"

CANBus::CANBus(PinName rd, PinName td, HWBRIDGE::CANMsgMap *rxStreamedMsgMap, HWBRIDGE::CANMsgMap *txStreamedMsgMap,
               const CANMsg::CANMsgHandlerMap *rxOneShotMsgHandler, uint32_t filterID, uint32_t filterMask,
               uint32_t freqency_hz)
    : CAN(rd, td, freqency_hz),
      m_rxPostmanThread(RX_POSTMAN_THREAD_PRIORITY),
      m_rxClientThread(RX_CLIENT_THREAD_PRIORITY),
      m_txProcessorThread(TX_PROCESSOR_THREAD_PRIORITY),
      m_rxStreamedMsgMap(rxStreamedMsgMap),
      m_txStreamedMsgMap(txStreamedMsgMap),
      m_rxOneShotMsgHandler(rxOneShotMsgHandler) {
  this->filter(filterID, filterMask, CANStandard);
  m_rxPostmanThread.start(callback(&m_rxEventQueue, &EventQueue::dispatch_forever));
  m_rxClientThread.start(callback(this, &CANBus::rxClient));
  m_txProcessorThread.start(callback(this, &CANBus::txProcessor));
  this->attach(callback(this, &CANBus::rxISR), CANBus::RxIrq);
}

void CANBus::rxISR(void) {
  can_irq_set(&_can, IRQ_RX, false);
  m_rxEventQueue.call(callback(this, &CANBus::rxPostman));
}

void CANBus::rxPostman(void) {
  CANMsg msg;
  // this loop is needed to avoid missing msg received between turning off the IRQ and turning it back on
  while (this->read(msg)) {
    CANMsg *mail = m_rxMailbox.try_alloc();  // no wait

    if (mail != nullptr) {
      *mail = msg;
      MBED_ASSERT(m_rxMailbox.put(mail) == osOK);
    } else {
      MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_BUFFER_FULL), "CAN RX mailbox full");
    }
  }
  can_irq_set(&_can, IRQ_RX, true);
}

void CANBus::rxClient(void) {
  while (true) {
    CANMsg *mail = nullptr;

    do {
      mail = m_rxMailbox.try_get();  // TODO: try_get_for was not working. Investigate why and use it
      ThisThread::sleep_for(1ms);
    } while (mail == nullptr);

    // Extract message ID and payload
    HWBRIDGE::CANID msgID = mail->getID();
    HWBRIDGE::CANMsgData_t msgData;
    mail->getPayload(msgData);

    MBED_ASSERT(m_rxMailbox.free(mail) == osOK);

    // If message is one-shot, process message
    if (m_rxOneShotMsgHandler->find(msgID) != m_rxOneShotMsgHandler->end()) {
      CANMsg msg;
      msg.setID(msgID);
      msg.setPayload(msgData);
      m_rxOneShotMsgHandler->at(msgID)(msg);
    }

    // Otherwise message is streamed - extract message signals and put in RX message map
    if (!HWBRIDGE::unpackCANMsg(msgData.raw, msgID, m_rxStreamedMsgMap)) {
      MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                   "CAN RX message unpacking failed");
    }
  }
}

void CANBus::txProcessor(void) {
  while (true) {
    CANMsg *mail = nullptr;

    // Send all one-shots messages that were queued
    while ((mail = m_txMailboxOneShot.try_get()) != nullptr) {
      if (!this->write(*mail)) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED), "CAN TX write failed");
      }
      ThisThread::sleep_for(TX_INTERDELAY);
    }

    // Send all streamed messages
    for (auto it = m_txStreamedMsgMap->begin(); it != m_txStreamedMsgMap->end(); it++) {
      HWBRIDGE::CANID msgID = it->first;

      // TODO: figure out how to set msg payload to correct size
      HWBRIDGE::CANMsgData_t msgData = {0};
      size_t len                     = 0;
      if (HWBRIDGE::packCANMsg(msgData.raw, msgID, m_txStreamedMsgMap, len)) {
        // Send message
        CANMsg msg;
        msg.setID(msgID);
        msg.setPayload(msgData);
        this->write(msg);
        ThisThread::sleep_for(TX_INTERDELAY);
      } else {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_DATA_DETECTED),
                     "CAN RX message packing failed");
      }
    }

    ThisThread::sleep_for(TX_PERIOD);
  }
}

bool CANBus::postMessageOneShot(CANMsg *msg) {
  bool success = false;
  if (msg) {
    CANMsg *mail = m_txMailboxOneShot.try_alloc_for(1ms);
    if (mail) {
      *mail = *msg;
      MBED_ASSERT(m_txMailboxOneShot.put(mail) == osOK);
      success = true;
    }
  }
  return success;
}

bool CANBus::updateStreamedSignal(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                                  HWBRIDGE::CANSignalValue_t signalValue) {
  return m_txStreamedMsgMap->setSignalValue(msgID, signalName, signalValue);
}

bool CANBus::readStreamedSignal(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                                HWBRIDGE::CANSignalValue_t &signalValue) {
  return m_rxStreamedMsgMap->getSignalValue(msgID, signalName, signalValue);
}

can_t *CANBus::getHandle() {
  return &_can;
}