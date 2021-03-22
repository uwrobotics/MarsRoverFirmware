#include "CANBus.h"

CANBus::CANBus(PinName rd, PinName td, CANMsg::CANMsgMap *rxMsgMap, CANMsg::CANMsgMap *txMsgMap,
               CANMsg::CANMsgHandlerMap *rxMsgHandlerMap, CANSignal::CANSignalMap *txSignalMap, uint32_t freqency_hz)
    : CAN(rd, td, freqency_hz),
      m_rxPostmanThread(RX_POSTMAN_THREAD_PRIORITY),
      m_rxProcessorThread(RX_PROCESSOR_THREAD_PRIORITY),
      m_txProcessorThread(TX_PROCESSOR_THREAD_PRIORITY),
      m_rxMsgMap(rxMsgMap),
      m_txMsgMap(txMsgMap),
      m_rxMsgHandlerMap(rxMsgHandlerMap),
      m_txSignalMap(txSignalMap) {
  m_rxPostmanThread.start(callback(&m_rxEventQueue, &EventQueue::dispatch_forever));
  m_rxProcessorThread.start(callback(this, &CANBus::rxProcessor));
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

void CANBus::rxProcessor(void) {
  CANMsg *mail = nullptr;

  do {
    mail = m_rxMailbox.try_get();  // TODO: try_get_for was not working. Investigate why and use it
    ThisThread::sleep_for(1ms);
  } while (mail == nullptr);

  MBED_ASSERT(mail != nullptr);
  m_rxMsgHandlerMap->at(mail->getID())(*mail);
  MBED_ASSERT(m_rxMailbox.free(mail) == osOK);
}

void CANBus::txProcessor(void) {
  CANMsg *mail = nullptr;

  // Send all one-shots messages that were queued
  while ((mail = m_txMailboxOneShot.try_get()) != nullptr) {
    this->write(*mail);
    ThisThread::sleep_for(TX_INTERDELAY);
  }

  // Send all streamed messages
  // for (auto it = m_txMsgMap->begin(); it != m_txMsgMap->end(); it++) {
  //   this->write(it->second);
  //   ThisThread::sleep_for(TX_INTERDELAY);
  // }

  // Send all streamed messages
  // For each message in signal map
  for (auto it = m_txSignalMap->begin(); it != m_txSignalMap->end(); it++) {
    uint16_t msgID = it->first;

    uint8_t raw[8] = {0};
    packCANMsg(raw, msgID, &(it->second));

    // Send message
    CANMsg msg;
    msg.setID((HWBRIDGE::CANID)msgID);
    msg.setPayload(raw);
    this->write(msg);
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

void CANBus::postMessageStreamed(CANMsg *msg) {
  m_txMsgMap->at(msg->getID()) = *msg;
}

bool CANBus::updateSignal(double value, CAN_SIGNAL_NAME signalName, uint16_t msgID) {
  // Find signal in message
  for (CANSignal &signal : (*m_txSignalMap).at(msgID).value()) {
    if (signal.getName() == signalName) {
      signal.setValue(value);
      return true;
    }
  }
  return false;
}

can_t *CANBus::getHandle() {
  return &_can;
}