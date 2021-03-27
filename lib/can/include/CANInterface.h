#pragma once

#include "CANBus.h"
#include "CANMsg.h"
#include "hw_bridge.h"

class CANInterface {
 public:
  typedef struct {
    // CAN bus pins
    PinName can1_RX;
    PinName can1_TX;
    PinName can2_RX;
    PinName can2_TX;

    // Message maps and handlers
    HWBRIDGE::CANMsgMap *rxStreamedMsgMap;
    HWBRIDGE::CANMsgMap *txStreamedMsgMap;
    const CANMsg::CANMsgHandlerMap *rxOneShotMsgHandler;

    // HW filter
    HWBRIDGE::CANFILTER filterID = HWBRIDGE::CANFILTER::NO_FILTER;
    CANFormat filterFormat       = CANStandard;
    uint16_t filterMask          = 0;

    // Bus frequency
    uint32_t frequency_hz = HWBRIDGE::ROVER_CANBUS_FREQUENCY_HZ;
  } Config;

  // Initialize CAN interface
  CANInterface(const Config &config);

  // Queue up a one shot message to be sent
  bool sendOneShotMessage(CANMsg &msg);

  // Update a TX CAN signal
  bool updateStreamedSignal(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                            HWBRIDGE::CANSignalValue_t signalValue);

  // Read a RX CAN signal
  bool readStreamedSignal(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                          HWBRIDGE::CANSignalValue_t &signalValue);

  // Switch CAN bus
  void switchCANBus(HWBRIDGE::CANBUSID canBusID);

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

  CANBus m_CANBus1;
  CANBus m_CANBus2;
  CANBus *m_activeCANBus;

  Thread m_rxPostmanThread;
  Thread m_rxClientThread;
  Thread m_txProcessorThread;

  Mail<CANMsg, 100> m_rxMailbox;
  Mail<CANMsg, 32> m_txMailboxOneShot;
  EventQueue m_rxEventQueue;

  HWBRIDGE::CANMsgMap *m_rxStreamedMsgMap;
  HWBRIDGE::CANMsgMap *m_txStreamedMsgMap;

  const CANMsg::CANMsgHandlerMap *m_rxOneShotMsgHandler;
};
