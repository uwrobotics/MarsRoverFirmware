#include "GPS.h"
GPS::GPS(PinName tx, PinName rx, uint32_t baud_rate) : m_uart(tx, rx, DEFAULT_BAUD_RATE) {
  m_rollingChecksumA  = 0;
  m_rollingChecksumB  = 0;
  m_ubxFrameCounter   = 0;
  m_ignoreThisPayload = false;
  m_ack_flag          = false;
  m_nack_flag         = false;
  m_target_class      = UBX_CLASS_NAV;  // default class target
  m_target_msgID      = UBX_NAV_PVT;    // default id target
  m_buf_write = m_buf_read = m_buffer.begin();
  setSerialRate(baud_rate, COM_PORT_UART1);
  m_uart.baud(baud_rate);
  wait_ms(50);  // ACK lost during baud rate change
  m_uart.attach(callback(this, &GPS::onByteReceive));
}

void GPS::configure() {
  // TODO: Determine GNSS constellation configuration (necessary? best config?)
  // allow uart1 to output UBX messages
  setPortOutput(COM_PORT_UART1, COM_TYPE_UBX);
  if (!waitForAck(UBX_CLASS_CFG, UBX_CFG_PRT)) {
    // TODO: handle NACK response
  }
  setNavigationFrequency(NAV_FREQUENCY);
  if (!waitForAck(UBX_CLASS_CFG, UBX_CFG_RATE)) {
    // TODO: handle NACK response
  }
  // configure UBX_NAV_PVT to output from uart 1 every navigation cycle
  configureMessage(UBX_CLASS_NAV, UBX_NAV_PVT, COM_PORT_UART1, 1);
  if (!waitForAck(UBX_CLASS_CFG, UBX_CFG_MSG)) {
    // TODO: handle NACK response
  }
  // turn default NEMA messages off
  for (const uint8_t message : DEFAULT_ENABLED_NMEA) {
    configureMessage(UBX_CLASS_NMEA, message, COM_PORT_UART1, 0);  // 0 sendRate message off
    if (!waitForAck(UBX_CLASS_CFG, UBX_CFG_MSG)) {
      // TODO: handle NACK response
    }
  }
}

void GPS::process_byte_from_buffer() {
  if (m_buf_read == m_buffer.end()) {
    m_buf_read = m_buffer.begin();
  }
  uint8_t tmp = *m_buf_read;
  ++m_buf_read;
  processNonPayload(tmp, (m_activePacketBuffer == UBLOX_PACKET_PACKETCFG
                              ? &m_packetCfg
                              : m_activePacketBuffer == UBLOX_PACKET_PACKETACK ? &m_packetAck : &m_packetBuf));
}

bool GPS::waitForAck(uint8_t msgClass, uint8_t msgID) {
  m_ack_flag     = false;  // clear flags
  m_nack_flag    = false;
  uint8_t tmpcls = m_target_class;
  uint8_t tmpID  = m_target_msgID;
  m_target_class = msgClass;
  m_target_msgID = msgID;
  m_ack_timeout.reset();
  m_ack_timeout.start();
  // TODO: need error checking for failed checksums, etc
  while (!m_ack_flag) {  // wait for ack
    if (m_nack_flag || m_ack_timeout.read_ms() > ACK_TIMEOUT_MS) {
      m_ack_timeout.stop();
      m_target_class = tmpcls;
      m_target_msgID = tmpID;
      return false;
    }
  }
  m_target_class = tmpcls;
  m_target_msgID = tmpID;
  return true;
}

void GPS::onByteReceive() {
  if (m_buf_write == m_buffer.end()) {
    m_buf_write = m_buffer.begin();
  }
  *m_buf_write = m_uart.getc();
  ++m_buf_write;
}

void GPS::sendSerialCommand(ubxPacket *outgoingUBX) {
  calcChecksum(outgoingUBX);
  // Write header bytes
  m_uart.putc(UBX_SYNCH_1);  //μ
  m_uart.putc(UBX_SYNCH_2);  // b
  m_uart.putc(outgoingUBX->cls);
  m_uart.putc(outgoingUBX->id);
  m_uart.putc(outgoingUBX->len & 0xFF);  // LSB
  m_uart.putc(outgoingUBX->len >> 8);    // MSB

  // Write payload
  m_uart.write(outgoingUBX->payload, outgoingUBX->len, NULL);

  // Write checksum
  m_uart.putc(outgoingUBX->checksumA);
  m_uart.putc(outgoingUBX->checksumB);
}

void GPS::processNonPayload(uint8_t incoming, ubxPacket *incomingUBX) {
  if (currentSentence == NONE) {
    if (incoming == UBX_SYNCH_1)  // first char of message
    {
      // start of sentence
      m_ubxFrameCounter   = 0;
      currentSentence     = UBX;
      m_packetBuf.counter = 0;
      m_ignoreThisPayload = false;
      // Store data in m_packetBuf until class known
      m_activePacketBuffer = UBLOX_PACKET_PACKETBUF;
    } else {
      // TODO: handle failure case (missed sentence start)
    }
  }
  // non-payload bytes if/else chain
  if (((m_ubxFrameCounter == 0) && (incoming != UBX_SYNCH_1)) ||
      ((m_ubxFrameCounter == 1) && (incoming != UBX_SYNCH_2)))
    currentSentence = NONE;         // error, reset sentence
  else if (m_ubxFrameCounter == 2)  // Class
  {
    m_rollingChecksumA       = 0;  // reset member states
    m_rollingChecksumB       = 0;
    m_packetBuf.counter      = 0;
    m_packetBuf.valid        = UBLOX_PACKET_VALIDITY_NOT_DEFINED;
    m_packetBuf.startingSpot = incomingUBX->startingSpot;
  } else if (m_ubxFrameCounter == 3) {  // ID
    // determine class
    if (m_packetBuf.cls != UBX_CLASS_ACK) {
      if ((m_packetBuf.cls == m_target_class) && (m_packetBuf.id == m_target_msgID)) {
        // not ACK, Class and ID match
        // copy data to proper buffer
        m_activePacketBuffer = UBLOX_PACKET_PACKETCFG;
        incomingUBX->cls     = m_packetBuf.cls;
        incomingUBX->id      = m_packetBuf.id;
        incomingUBX->counter = m_packetBuf.counter;
      } else {
        // not ACK or requested packet, so ignore it
        m_ignoreThisPayload = true;
      }
    }
  } else if (m_activePacketBuffer == UBLOX_PACKET_PACKETBUF && (m_ubxFrameCounter == 6 || m_ubxFrameCounter == 7)) {
    handleACK(incoming);
  }
  selectProcessBuffer(incoming, incomingUBX);
  m_ubxFrameCounter++;
}

// Given a character, file it away into the uxb packet structure
// Set valid to VALID or NOT_VALID once sentence is completely received and passes or fails CRC
// startingSpot can be set so we only record a subset of bytes within a larger packet.
void GPS::processPayload(uint8_t incoming, ubxPacket *incomingUBX) {
  // Note that incomingUBX->counter == (m_ubxFrameCounter - 2)
  // Add all incoming bytes to the rolling checksum
  // Stop at len+4, as these are the checksum bytes
  if (incomingUBX->counter < incomingUBX->len + 4) {
    addToChecksum(incoming);
  }
  if (incomingUBX->counter == 0) {
    incomingUBX->cls = incoming;
  } else if (incomingUBX->counter == 1) {
    incomingUBX->id = incoming;
  } else if (incomingUBX->counter == 2) {  // Len LSB
    incomingUBX->len = incoming;
  } else if (incomingUBX->counter == 3) {  // Len MSB
    incomingUBX->len |= incoming << 8;
  } else if (incomingUBX->counter == incomingUBX->len + 4) {  // ChecksumA
    incomingUBX->checksumA = incoming;
  } else if (incomingUBX->counter == incomingUBX->len + 5) {  // ChecksumB, last byte of sentence trasmission
    incomingUBX->checksumB = incoming;
    currentSentence        = NONE;  // next byte is new sentence
    validateChecksum(incomingUBX);

  } else {  // payload byte
    if (!m_ignoreThisPayload) {
      // fudge starting spot for UBX_PVT_NAV
      uint16_t tmp_startingSpot = incomingUBX->startingSpot;
      if (incomingUBX->cls == UBX_CLASS_NAV && incomingUBX->id == UBX_NAV_PVT) tmp_startingSpot = 0;
      // begin recording if counter goes past startingSpot
      if ((incomingUBX->counter - 4) >= tmp_startingSpot) {
        // check to see if we have room for this byte
        if (((incomingUBX->counter - 4) - tmp_startingSpot) < MAX_PAYLOAD_SIZE) {
          incomingUBX->payload[incomingUBX->counter - 4 - tmp_startingSpot] = incoming;
        }
      }
    }
  }
  incomingUBX->counter++;

  if (incomingUBX->counter == MAX_PAYLOAD_SIZE) {
    //(!)
    // TODO: look into error handling here
    currentSentence = NONE;  // reset, start new with next message
  }
}

void GPS::extractPacketData(ubxPacket *msg) {
  if (msg->id == UBX_NAV_PVT && msg->len == 92) {
    // Parse various byte fields
    m_nav_data.year       = extractInt(4);
    m_nav_data.month      = extractByte(6);
    m_nav_data.day        = extractByte(7);
    m_nav_data.hour       = extractByte(8);
    m_nav_data.minute     = extractByte(9);
    m_nav_data.second     = extractByte(10);
    m_nav_data.nanosecond = extractLong(16);  // Includes milliseconds

    m_nav_data.longitude       = extractLong(24);
    m_nav_data.latitude        = extractLong(28);
    m_nav_data.altitude        = extractLong(32);
    m_nav_data.groundSpeed     = extractLong(60);
    m_nav_data.headingOfMotion = extractLong(64);
  } else if (msg->cls == UBX_CLASS_ACK) {
    (msg->id == UBX_ACK_ACK ? m_ack_flag : m_nack_flag) = true;
  }
  // TODO: Cover Failure Case
  // note that we don't care about any other message types for our localization
}

void GPS::handleACK(uint8_t incoming) {
  if (m_ubxFrameCounter == 6) {
    // first payload byte (if ACK/NACK packet)
    if (m_packetBuf.len == 0) {  // should be impossible
      // if length is zero (!), this is first checksum byte
      m_packetBuf.checksumA = incoming;
    } else {
      m_packetBuf.payload[0] = incoming;
    }
  } else if (m_ubxFrameCounter == 7) {
    // second payload byte (if ACK/NACK packet)
    if (m_packetBuf.len == 0)  // should be impossible
    {
      // If length is zero (!), second checksum byte
      m_packetBuf.checksumB = incoming;
    } else if (m_packetBuf.len == 1) {  // first checksum byte
      m_packetBuf.checksumA = incoming;
    } else {  // payload byte
      m_packetBuf.payload[1] = incoming;
    }
    // now determine ACK/NACK
    if ((m_packetBuf.cls == UBX_CLASS_ACK)                // if ACK/NACK
        && (m_packetBuf.payload[0] == m_target_class)     // if class match
        && (m_packetBuf.payload[1] == m_target_msgID)) {  // if ID match
      if (m_packetBuf.len == 2) {
        // is the requested ACK
        m_activePacketBuffer   = UBLOX_PACKET_PACKETACK;
        m_packetAck.cls        = m_packetBuf.cls;
        m_packetAck.id         = m_packetBuf.id;
        m_packetAck.len        = m_packetBuf.len;
        m_packetAck.counter    = m_packetBuf.counter;
        m_packetAck.payload[0] = m_packetBuf.payload[0];
        m_packetAck.payload[1] = m_packetBuf.payload[1];
      } else {
        // TODO: cover error if length is not 2 (may impact waitForAck)
      }
    }
  } else {
    // should never happen
    // TODO: handle error here
  }
}

// Process incoming data using the proper buffer
void GPS::selectProcessBuffer(uint8_t incoming, ubxPacket *incomingUBX) {
  if (m_activePacketBuffer == UBLOX_PACKET_PACKETACK) {  // ACK/NACK
    processPayload(incoming, &m_packetAck);
  } else if (m_activePacketBuffer == UBLOX_PACKET_PACKETCFG) {  // data packet
    processPayload(incoming, incomingUBX);
  } else {  // unknown
    processPayload(incoming, &m_packetBuf);
  }
  m_ubxFrameCounter++;
}

void GPS::validateChecksum(ubxPacket *incomingUBX) {
  // CRC validate
  if ((incomingUBX->checksumA == m_rollingChecksumA) && (incomingUBX->checksumB == m_rollingChecksumB)) {
    incomingUBX->valid = UBLOX_PACKET_VALIDITY_VALID;  // Flag the packet as valid

    // verifying class and ID match for ACK
    if (((incomingUBX->cls == m_target_class) && (incomingUBX->id == m_target_msgID)) ||
        ((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->id == UBX_ACK_ACK) &&
         (incomingUBX->payload[0] == m_target_class) && (incomingUBX->payload[1] == m_target_msgID))) {
      // if match, set valid flag
      incomingUBX->classAndIDmatch = UBLOX_PACKET_VALIDITY_VALID;
    } else if ((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->id == UBX_ACK_NACK) &&
               (incomingUBX->payload[0] == m_target_class) && (incomingUBX->payload[1] == m_target_msgID)) {
      // if match, set NACK flag
      incomingUBX->classAndIDmatch = UBLOX_PACKET_NOTACKNOWLEDGED;
    }

    // complete valid packet, process it
    if (!m_ignoreThisPayload) {
      extractPacketData(incomingUBX);
    }
  } else {  // Checksum failure
    incomingUBX->valid = UBLOX_PACKET_VALIDITY_NOT_VALID;

    // Not sure if I like the idea behind this..., will likely remove once I verify that I can
    // assumes class/ID bytes didn't cause CRC failure
    // TODO: Determine if this can be removed without breaking things
    if (((incomingUBX->cls == m_target_class) && (incomingUBX->id == m_target_msgID)) ||
        ((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->payload[0] == m_target_class) &&
         (incomingUBX->payload[1] == m_target_msgID))) {
      // if match, set not valid flag
      incomingUBX->classAndIDmatch = UBLOX_PACKET_VALIDITY_NOT_VALID;
    }
  }
}

// checksum helpers
void GPS::calcChecksum(ubxPacket *msg) {
  msg->checksumA = 0;
  msg->checksumB = 0;

  msg->checksumA += msg->cls;
  msg->checksumB += msg->checksumA;

  msg->checksumA += msg->id;
  msg->checksumB += msg->checksumA;

  msg->checksumA += (msg->len & 0xFF);
  msg->checksumB += msg->checksumA;

  msg->checksumA += (msg->len >> 8);
  msg->checksumB += msg->checksumA;

  for (uint16_t i = 0; i < msg->len; i++) {
    msg->checksumA += msg->payload[i];
    msg->checksumB += msg->checksumA;
  }
}
void GPS::addToChecksum(uint8_t incoming) {
  m_rollingChecksumA += incoming;
  m_rollingChecksumB += m_rollingChecksumA;
}

void GPS::setSerialRate(uint32_t baudrate, uint8_t uartPort) {
  getPortSettings(uartPort);  // load payloadCfg with current port settings
  m_packetCfg.cls          = UBX_CLASS_CFG;
  m_packetCfg.id           = UBX_CFG_PRT;
  m_packetCfg.len          = 20;
  m_packetCfg.startingSpot = 0;

  // change necessary msg data
  payloadCfg[8]  = baudrate;
  payloadCfg[9]  = baudrate >> 8;
  payloadCfg[10] = baudrate >> 16;
  payloadCfg[11] = baudrate >> 24;

  sendSerialCommand(&m_packetCfg);
}

// Configure a given port to output UBX, NMEA, and/or RCTM3
// Port:     	use COM_PORT_ constants
// Settings: 	use COM_TYPE_ constants
void GPS::setPortOutput(uint8_t portID, uint8_t outStreamSettings) {
  getPortSettings(portID);  // load payloadCfg with current port settings
  m_packetCfg.cls          = UBX_CLASS_CFG;
  m_packetCfg.id           = UBX_CFG_PRT;
  m_packetCfg.len          = 20;
  m_packetCfg.startingSpot = 0;

  // change necessary msg data
  payloadCfg[14] = outStreamSettings;
  sendSerialCommand(&m_packetCfg);
}

// Configure a given port to accept UBX, NMEA, RTCM3 or a combination thereof
// Port:     	use COM_PORT_ constants
// Settings: 	use COM_TYPE_ constants
void GPS::setPortInput(uint8_t portID, uint8_t inStreamSettings) {
  getPortSettings(portID);  // load payloadCfg with current port settings
  m_packetCfg.cls          = UBX_CLASS_CFG;
  m_packetCfg.id           = UBX_CFG_PRT;
  m_packetCfg.len          = 20;
  m_packetCfg.startingSpot = 0;

  // change necessary msg data
  payloadCfg[12] = inStreamSettings;
  sendSerialCommand(&m_packetCfg);
}

void GPS::setNavigationFrequency(uint8_t navFreq) {
  m_packetCfg.cls          = UBX_CLASS_CFG;
  m_packetCfg.id           = UBX_CFG_RATE;
  m_packetCfg.len          = 0;
  m_packetCfg.startingSpot = 0;

  // load payloadCFG with current settings
  sendSerialCommand(&m_packetCfg);
  if (!waitForAck(UBX_CLASS_CFG, UBX_CFG_RATE)) {
    // TODO: handle failure case
  }

  uint16_t measurementRate = 1000 / navFreq;

  // payloadCfg is now loaded with current bytes. Change only the ones we need to
  payloadCfg[0] = measurementRate & 0xFF;  // measRate LSB
  payloadCfg[1] = measurementRate >> 8;    // measRate MSB

  sendSerialCommand(&m_packetCfg);
}

void GPS::configureMessage(uint8_t msgClass, uint8_t msgID, uint8_t portID, uint8_t sendRate) {
  m_packetCfg.cls          = UBX_CLASS_CFG;
  m_packetCfg.id           = UBX_CFG_MSG;
  m_packetCfg.len          = 8;
  m_packetCfg.startingSpot = 0;

  // Clear packet payload
  for (int x = 0; x < m_packetCfg.len; x++) {
    m_packetCfg.payload[x] = 0;
  }
  m_packetCfg.payload[0] = msgClass;
  m_packetCfg.payload[1] = msgID;
  // Send rate is how many cycles pass between each send
  // For example, if the rate of a navigation message is set to 2, the message is sent every 2nd navigation solution.
  m_packetCfg.payload[2 + portID] = sendRate;

  sendSerialCommand(&m_packetCfg);
}

void GPS::saveConfiguration()
// saves config in receiver battery backed RAM
// saved config is automatically loaded on startup
{
  m_packetCfg.cls          = UBX_CLASS_CFG;
  m_packetCfg.id           = UBX_CFG_CFG;
  m_packetCfg.len          = 12;
  m_packetCfg.startingSpot = 0;

  // clear packet payload
  for (int x = 0; x < m_packetCfg.len; x++) {
    m_packetCfg.payload[x] = 0;
  }

  m_packetCfg.payload[4] = 0xFF;  // set aveMask bits
  m_packetCfg.payload[5] = 0xFF;

  sendSerialCommand(&m_packetCfg);
}

void GPS::getPortSettings(uint8_t portID) {
  m_packetCfg.cls          = UBX_CLASS_CFG;
  m_packetCfg.id           = UBX_CFG_PRT;
  m_packetCfg.len          = 1;
  m_packetCfg.startingSpot = 0;
  payloadCfg[0]            = portID;
  sendSerialCommand(&m_packetCfg);
  if (!waitForAck(UBX_CLASS_CFG, UBX_CFG_PRT)) {
    // TODO: handle failure case
  }
}

// Helper functions for extracting byte data
uint32_t GPS::extractLong(uint8_t spotToStart) {
  uint32_t val = 0;
  val |= (uint32_t)payloadCfg[spotToStart + 0] << 8 * 0;
  val |= (uint32_t)payloadCfg[spotToStart + 1] << 8 * 1;
  val |= (uint32_t)payloadCfg[spotToStart + 2] << 8 * 2;
  val |= (uint32_t)payloadCfg[spotToStart + 3] << 8 * 3;
  return (val);
}

uint16_t GPS::extractInt(uint8_t spotToStart) {
  uint16_t val = 0;
  val |= (uint16_t)payloadCfg[spotToStart + 0] << 8 * 0;
  val |= (uint16_t)payloadCfg[spotToStart + 1] << 8 * 1;
  return (val);
}

uint8_t GPS::extractByte(uint8_t spotToStart) {
  return (payloadCfg[spotToStart]);
}

double GPS::getLat() {
  return m_nav_data.latitude * 10E-7;  // degrees
}
double GPS::getLong() {
  return m_nav_data.longitude * 10E-7;  // degrees
}
double GPS::getAlt() {
  return m_nav_data.altitude * 10E-3;  // meters above elipsoid
}
double GPS::getTime() {  // seconds since midnight
  return m_nav_data.hour * 3600 + m_nav_data.minute * 60 + m_nav_data.second + m_nav_data.nanosecond * 10E-9;
}
