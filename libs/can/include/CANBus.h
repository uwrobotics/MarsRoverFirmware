#pragma once

#include "CANAutogenWrapper.h"
#include "CANMsg.h"
#include "CANSignal.h"
#include "LookupTable.h"
#include "hw_bridge.h"
#include "mbed.h"

class CANBus : public CAN {
 public:
  // Initialize CAN bus interface TODO: add filter here
  CANBus(PinName rd, PinName td, CANMsg::CANMsgMap *rxMsgMap, CANMsg::CANMsgMap *txMsgMap,
         CANMsg::CANMsgHandlerMap *rxMsgHandlerMap, CANSignal::CANSignalMap *txSignalMap, uint32_t freqency_hz);

  // Post TX message to be sent
  bool postMessageOneShot(CANMsg *msg);
  void postMessageStreamed(CANMsg *msg);

  bool updateSignal(double value, CAN_SIGNAL_NAME signalName, uint16_t msgID);

  can_t *getHandle();

 private:
  static constexpr osPriority RX_POSTMAN_THREAD_PRIORITY   = osPriorityRealtime;
  static constexpr osPriority RX_PROCESSOR_THREAD_PRIORITY = osPriorityAboveNormal;
  static constexpr osPriority TX_PROCESSOR_THREAD_PRIORITY = osPriorityBelowNormal;
  static constexpr std::chrono::milliseconds TX_INTERDELAY = 1ms;

  void rxISR(void);
  void rxPostman(void);
  void rxProcessor(void);
  void txProcessor(void);

  Thread m_rxPostmanThread;
  Thread m_rxProcessorThread;
  Thread m_txProcessorThread;

  Mail<CANMsg, 100> m_rxMailbox;
  Mail<CANMsg, 32> m_txMailboxOneShot;
  EventQueue m_rxEventQueue;

  CANMsg::CANMsgMap *m_rxMsgMap;
  CANMsg::CANMsgMap *m_txMsgMap;

  CANMsg::CANMsgHandlerMap *m_rxMsgHandlerMap;

  CANSignal::CANSignalMap *m_txSignalMap;
};
