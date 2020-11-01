#pragma once

#include "LookupTable.h"
#include "hw_bridge.h"
#include "mbed.h"

class CANMsg : public CANMessage {
 private:
  using CAN_Message::id;
 public:
  using CANMsgHandler = mbed_error_status_t (*)(CANMsg &);
  //typedef LookupTable<uint16_t, CANMsg::CANMsgHandler> CANMsgHandlerMap;
  using CANMsgHandlerMap = LookupTable<HWBRIDGE::CANID, CANMsg::CANMsgHandler, +[](CANMsg &)->mbed_error_status_t {return MBED_SUCCESS;}>;

  template <class T>
  union CANPayload {
    unsigned char data[8];
    T value;
  };

  /** Creates empty CAN message.
   */
  CANMsg() : CANMessage() {}

  /** Creates CAN message with specific content.
   */
  CANMsg(HWBRIDGE::CANID _id, const char *_data, char _len = 8, CANType _type = CANData,
         CANFormat _format = CANStandard)
      : CANMessage(static_cast<uint16_t>(_id), _data, _len, _type, _format) {}

  /** Creates CAN remote message.
   */
  CANMsg(HWBRIDGE::CANID _id, CANFormat _format = CANStandard) : CANMessage(static_cast<uint16_t>(_id), _format) {}

  /** Sets the ID for a CAN messages
   */
  void setID(const HWBRIDGE::CANID newID) {
    id = static_cast<uint16_t>(newID);
  }

  /** Returns the ID of the CAN message
   */
  HWBRIDGE::CANID getID() const {
    return static_cast<HWBRIDGE::CANID>(id);
  }

  /** Clears CAN message content
   */
  void clear(void) {
    len    = 0;
    type   = CANData;
    format = CANStandard;
    id     = 0;
    memset(data, 0, 8);
  };

  /** Set the payload data
   */
  template <class T>
  void setPayload(const T value) {
    MBED_ASSERT_WARN(sizeof(T) <= 8);  // Make sure input type fits
    CANPayload<T> *payload = (CANPayload<T> *)&data;
    payload->value         = value;
    len                    = sizeof(T);
  }

  /** Get the payload data
   */
  template <class T>
  void getPayload(T &val) {
    MBED_ASSERT_WARN(len = sizeof(T));  // Make sure output fits
    CANPayload<T> *payload = (CANPayload<T> *)&data;
    val                    = payload->value;
  }

  /** Append operator: Appends data (value) to CAN message
   */
  template <class T>
  CANMsg &operator<<(const T val) {
    MBED_ASSERT_WARN(len + sizeof(T) <= 8);
    *reinterpret_cast<T *>(&data[len]) = val;
    len += sizeof(T);
    return *this;
  }

  /** Extract operator: Extracts data (value) from CAN message
   */
  template <class T>
  CANMsg &operator>>(T &val) {
    MBED_ASSERT_WARN(sizeof(T) <= len);
    val = *reinterpret_cast<T *>(&data[0]);
    len -= sizeof(T);
    memcpy(data, data + sizeof(T), len);
    return *this;
  }
};
