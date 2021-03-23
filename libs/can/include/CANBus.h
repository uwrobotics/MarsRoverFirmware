#pragma once

#include <unordered_set>

#include "CANAutogenWrapper.h"
#include "CANMsg.h"
#include "hw_bridge.h"
#include "mbed.h"

// TODO: where to put this
typedef struct {
  uint8_t raw[8];
} CANMsgData_t;

class CANBus : public CAN {
 public:
  using CANMsgIDList = const std::unordered_set<HWBRIDGE::CANMsgID_t>;

  // Initialize CAN bus interface TODO: add filter here
  CANBus(PinName rd, PinName td, CANSignalMap *rxSignalMap, CANSignalMap *txSignalMap, CANMsgIDList *rxOneShotMsgIDs,
         uint32_t freqency_hz);

  // Post one shot message to be sent
  bool postMessageOneShot(CANMsg *msg);

  // Update a TX CAN signal
  bool updateSignal(HWBRIDGE::CANSIGNALNAME signalName, uint16_t msgID, double value);

  can_t *getHandle();

 private:
  static constexpr osPriority RX_POSTMAN_THREAD_PRIORITY   = osPriorityRealtime;
  static constexpr osPriority RX_CLIENT_THREAD_PRIORITY    = osPriorityAboveNormal;
  static constexpr osPriority TX_PROCESSOR_THREAD_PRIORITY = osPriorityBelowNormal;
  static constexpr std::chrono::milliseconds TX_INTERDELAY = 1ms;
  static constexpr std::chrono::milliseconds TX_PERIOD     = 10ms;

  void rxISR(void);
  void rxPostman(void);
  void rxClient(void);
  void txProcessor(void);

  Thread m_rxPostmanThread;
  Thread m_rxClientThread;
  Thread m_txProcessorThread;

  Mail<CANMsg, 100> m_rxMailbox;
  Mail<CANMsg, 32> m_txMailboxOneShot;
  EventQueue m_rxEventQueue;

  CANSignalMap *m_rxSignalMap;
  CANSignalMap *m_txSignalMap;

  CANMsgIDList *m_rxOneShotMsgIDs;
};
