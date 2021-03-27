#pragma once

#include "LookupTable.h"
#include "hw_bridge.h"
#include "mbed.h"

class CANMsg : public CANMessage {
 private:
  using CAN_Message::id;

  static mbed_error_status_t defaultCANMsgHandler(CANMsg &msg) {
    MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_ARGUMENT),
                 "Invalid key to CANMsgHandlerMap");
    return MBED_ERROR_CODE_INVALID_ARGUMENT;
  }

 public:
  using CANMsgHandler    = mbed_error_status_t (*)(CANMsg &);
  using CANMsgHandlerMap = Utility::LookupTable<HWBRIDGE::CANID, CANMsg::CANMsgHandler, &defaultCANMsgHandler>;

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
    this->id = static_cast<uint16_t>(newID);
  }

  /** Returns the ID of the CAN message
   */
  HWBRIDGE::CANID getID() const {
    return static_cast<HWBRIDGE::CANID>(this->id);
  }

  /** Clears CAN message content
   */
  void clear(void) {
    this->len    = 0;
    this->type   = CANData;
    this->format = CANStandard;
    this->id     = 0;
    memset(this->data, 0, 8);
  };

  /** Set the payload data
   */
  template <class T>
  void setPayload(const T value) {
    MBED_ASSERT(sizeof(T) <= 8);  // Make sure input type fits
    CANPayload<T> *payload = (CANPayload<T> *)&data;
    payload->value         = value;
    this->len              = sizeof(T);
  }

  /** Set the payload data with custom length
   */
  template <class T>
  void setPayload(const T value, size_t length) {
    setPayload(value);
    len = length;
  }

  /** Get the payload data
   */
  template <class T>
  void getPayload(T &val) {
    MBED_ASSERT(this->len = sizeof(T));  // Make sure output fits
    CANPayload<T> *payload = (CANPayload<T> *)&data;
    val                    = payload->value;
  }
};
