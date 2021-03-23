#include "CANBus.h"

CANBus::CANBus(PinName rd, PinName td, CANSignalMap *rxSignalMap, CANSignalMap *txSignalMap,
               CANMsgIDList *rxOneShotMsgIDs, uint32_t freqency_hz)
    : CAN(rd, td, freqency_hz),
      m_rxPostmanThread(RX_POSTMAN_THREAD_PRIORITY),
      m_rxClientThread(RX_CLIENT_THREAD_PRIORITY),
      m_txProcessorThread(TX_PROCESSOR_THREAD_PRIORITY),
      m_rxSignalMap(rxSignalMap),
      m_txSignalMap(txSignalMap),
      m_rxOneShotMsgIDs(rxOneShotMsgIDs) {
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
    // TODO: Handle mail related errors better
    CANMsg *mail = m_rxMailbox.try_alloc_for(1ms);
    MBED_ASSERT(mail != nullptr);
    *mail = msg;
    m_rxMailbox.put(mail);
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

    MBED_ASSERT(mail != nullptr);

    HWBRIDGE::CANMsgID_t msgID = (HWBRIDGE::CANMsgID_t)mail->getID();
    CANMsgData_t msgData;
    mail->getPayload(msgData);

    // If message is one-shot, process message
    if (m_rxOneShotMsgIDs->find(msgID) != m_rxOneShotMsgIDs->end()) {
      // TODO: process one-shot msg
    }

    // Otherwise message is streamed
    // Extract message signals
    unpackCANMsg(msgData.raw, msgID, m_rxSignalMap);

    MBED_ASSERT(m_rxMailbox.free(mail) == osOK);
  }
}

void CANBus::txProcessor(void) {
  while (true) {
    CANMsg *mail = nullptr;

    // Send all one-shots messages that were queued
    while ((mail = m_txMailboxOneShot.try_get()) != nullptr) {
      this->write(*mail);
      ThisThread::sleep_for(TX_INTERDELAY);
    }

    // Send all streamed messages
    for (auto it = m_txSignalMap->begin(); it != m_txSignalMap->end(); it++) {
      HWBRIDGE::CANMsgID_t msgID = it->first;

      // TODO: does it have to be 8 bytes?
      CANMsgData_t msgData = {0};
      packCANMsg(msgData.raw, msgID, m_txSignalMap);

      // Send message
      CANMsg msg;
      msg.setID((HWBRIDGE::CANID)msgID);
      msg.setPayload(msgData);
      this->write(msg);
      ThisThread::sleep_for(TX_INTERDELAY);
    }

    ThisThread::sleep_for(TX_PERIOD);
  }
}

bool CANBus::postMessageOneShot(CANMsg *msg) {
  CANMsg *mail = m_txMailboxOneShot.try_alloc_for(1ms);
  if (mail) {
    *mail = *msg;
    m_txMailboxOneShot.put(mail);
  }
  return (mail != nullptr);
}

bool CANBus::updateSignal(HWBRIDGE::CANSIGNALNAME signalName, uint16_t msgID, double value) {
  // Find signal in message
  if (m_txSignalMap->find(msgID) != m_txSignalMap->end()) {
    CANSignalLUT *signals = &(m_txSignalMap->at(msgID));

    if (signals->find(signalName) != signals->end()) {
      (*signals)[signalName] = value;
      return true;
    }
  }
  return false;
}

can_t *CANBus::getHandle() {
  return &_can;
}