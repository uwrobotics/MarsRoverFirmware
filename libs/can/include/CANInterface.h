#pragma once

#include "CANBus.h"
#include "CANMsg.h"
#include "hw_bridge.h"
#include "mbed.h"

class CANInterface {
 public:
  typedef struct {
    // CAN bus pins
    PinName can1_RX;
    PinName can1_TX;
    PinName can2_RX;
    PinName can2_TX;

    // Message maps and handlers
    HWBRIDGE::CANMsgMap *rxMsgMap;
    HWBRIDGE::CANMsgMap *txMsgMap;
    const CANMsg::CANMsgHandlerMap *rxOneShotMsgHandler;

    // Bus frequency
    uint32_t frequency_hz = HWBRIDGE::ROVER_CANBUS_FREQUENCY_HZ;
  } Config;

  // Initialize CAN interface
  CANInterface(const Config &config);

  // Queue up a one shot message to be sent
  bool sendOneShotMessage(CANMsg &msg, Kernel::Clock::duration_u32 timeout);

  // Update a TX CAN signal
  bool setTXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t signalValue); //here
// PDB_setLEDMatrix. 
  // Read a RX CAN signal
  bool getRXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t &signalValue); //here

  // Switch CAN bus
  bool switchCANBus(HWBRIDGE::CANBUSID canBusID);

  // Set CAN bus hw filter
  bool setFilter(HWBRIDGE::CANFILTER filter, CANFormat format = CANStandard,
                 uint16_t mask = HWBRIDGE::ROVER_CANID_FILTER_MASK, int handle = 0);

  // For diagnostic purposes
  uint32_t getNumStreamedMsgsReceived(void);
  uint32_t getNumOneShotMsgsReceived(void);
  uint32_t getNumStreamedMsgsSent(void);
  uint32_t getNumOneShotMsgsSent(void);

  uint16_t getNumCANRXFaults(void);
  uint16_t getNumCANTXFaults(void);

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

  Mutex m_rxMutex;
  Mutex m_txMutex;

  Mail<CANMsg, 100> m_rxMailbox;
  Mail<CANMsg, 32> m_txMailboxOneShot;
  EventQueue m_rxEventQueue;

  HWBRIDGE::CANMsgMap *m_rxMsgMap;
  HWBRIDGE::CANMsgMap *m_txMsgMap;

  const CANMsg::CANMsgHandlerMap *m_rxOneShotMsgHandler;

  // For diagnostic purposes
  uint32_t m_numStreamedMsgsReceived;
  uint32_t m_numOneShotMsgsReceived;
  uint32_t m_numStreamedMsgsSent;
  uint32_t m_numOneShotMsgsSent;

  uint16_t m_numCANRXFaults;
  uint16_t m_numCANTXFaults;
};
