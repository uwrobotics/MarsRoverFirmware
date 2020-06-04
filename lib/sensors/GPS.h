/*
 * UBlox SAM-M8Q-0-10 GPS Sensor
 * SparkFun Arduino Library: https://github.com/sparkfun/SparkFun_Ublox_Arduino_Library/tree/master/src
 * Datasheet: https://www.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification:
 * https://www.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_%28UBX-13003221%29.pdf
 */

/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to
do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <array>

#include "mbed.h"
#pragma once

// TODO remove unused constants when driver finalized
// TODO: put these constants in a different header (maybe?)

// Refresh frequency for new position data, in Hz
// TODO: determine what frequency we want
static constexpr uint8_t NAV_FREQUENCY = 5;
static constexpr int ACK_TIMEOUT_MS    = 2000;

static constexpr uint32_t BUFFER_SIZE = 100;

// ubxPacket validity
typedef enum {
  UBLOX_PACKET_VALIDITY_NOT_VALID,
  UBLOX_PACKET_VALIDITY_VALID,
  UBLOX_PACKET_VALIDITY_NOT_DEFINED,
  UBLOX_PACKET_NOTACKNOWLEDGED  // This indicates that we received a NACK
} ublox_packet_validity_e;

// Identify which packet buffer is in use:
// packetCfg (or a custom packet), packetAck or m_packetBuf
typedef enum { UBLOX_PACKET_PACKETCFG, UBLOX_PACKET_PACKETACK, UBLOX_PACKET_PACKETBUF } ublox_packet_buffer_e;

// header constants (first 2 message bytes)
static constexpr uint8_t UBX_SYNCH_1 = 0xB5;  // mu (greek letter)
static constexpr uint8_t UBX_SYNCH_2 = 0x62;  // b

// receiver default baud rate
static constexpr uint32_t DEFAULT_BAUD_RATE = 9600;

// used in config to disable the default messages
static constexpr std::array<const uint8_t, 7> DEFAULT_ENABLED_NMEA = {
    0x00,  // GxGGA (Global positioning system fix data)
    0x01,  // GxGLL (latitude and long, whith time of position fix and status)
    0x02,  // GxGSA (GNSS DOP and Active satellites)
    0x03,  // GxGSV (GNSS satellites in view)
    0x04,  // GxRMC (Recommended minimum data)
    0x05,  // GxVTG (course over ground and Ground speed)
    0x41   // GxTXT (text transmission)
};

static constexpr int MAX_PAYLOAD_SIZE = 768;
// message class constants
static constexpr uint8_t UBX_CLASS_NAV  = 0x01;  // Navigation Results Messages
static constexpr uint8_t UBX_CLASS_RXM  = 0x02;  // Receiver Manager Messages
static constexpr uint8_t UBX_CLASS_INF  = 0x04;  // Information Messages: Printf-Style Messages
static constexpr uint8_t UBX_CLASS_ACK  = 0x05;  // Ack/Nak Messages
static constexpr uint8_t UBX_CLASS_CFG  = 0x06;  // Configuration Input Messages
static constexpr uint8_t UBX_CLASS_UPD  = 0x09;  // Firmware Update Messages
static constexpr uint8_t UBX_CLASS_MON  = 0x0A;  // Monitoring Messages
static constexpr uint8_t UBX_CLASS_AID  = 0x0B;  //(NEO-M8P ONLY!!!) AssistNow Aiding Messages
static constexpr uint8_t UBX_CLASS_TIM  = 0x0D;  // Timing Messages
static constexpr uint8_t UBX_CLASS_ESF  = 0x10;  //(NEO-M8P ONLY!!!) External Sensor Fusion Messages
static constexpr uint8_t UBX_CLASS_MGA  = 0x13;  // Multiple GNSS Assistance Messages
static constexpr uint8_t UBX_CLASS_LOG  = 0x21;  // Logging Messages
static constexpr uint8_t UBX_CLASS_SEC  = 0x27;  // Security Feature Messages
static constexpr uint8_t UBX_CLASS_HNR  = 0x28;  //(NEO-M8P ONLY!!!) High Rate Navigation Results Messages
static constexpr uint8_t UBX_CLASS_NMEA = 0xF0;  // NMEA Strings: standard NMEA strings

// The following are used for configuration
static constexpr uint8_t UBX_CFG_ANT       = 0x13;  // Antenna Control Settings.
static constexpr uint8_t UBX_CFG_BATCH     = 0x93;  // Get/set data batching configuration.
static constexpr uint8_t UBX_CFG_CFG       = 0x09;  // Clear, Save, and Load Configurations.
static constexpr uint8_t UBX_CFG_DAT       = 0x06;  // Set User-defined Datum or The currently defined Datum
static constexpr uint8_t UBX_CFG_DGNSS     = 0x70;  // DGNSS configuration
static constexpr uint8_t UBX_CFG_GEOFENCE  = 0x69;  // Geofencing configuration. Used to configure a geofence
static constexpr uint8_t UBX_CFG_GNSS      = 0x3E;  // GNSS system configuration
static constexpr uint8_t UBX_CFG_INF       = 0x02;  // poll configuration for one protocol
static constexpr uint8_t UBX_CFG_ITFM      = 0x39;  // Jamming/Interference Monitor configuration
static constexpr uint8_t UBX_CFG_LOGFILTER = 0x47;  // Data Logger Configuration
static constexpr uint8_t UBX_CFG_MSG       = 0x01;  // Poll a message configuration, or Set Message Rate
static constexpr uint8_t UBX_CFG_NAV5      = 0x24;  // Navigation Engine Settings.
static constexpr uint8_t UBX_CFG_NAVX5     = 0x23;  // Navigation Engine Expert Settings
static constexpr uint8_t UBX_CFG_NMEA      = 0x17;  // Extended NMEA protocol configuration V1
static constexpr uint8_t UBX_CFG_ODO       = 0x1E;  // Odometer, Low-speed COG Engine Settings
static constexpr uint8_t UBX_CFG_PM2       = 0x3B;  // Extended power management configuration
static constexpr uint8_t UBX_CFG_PMS       = 0x86;  // Power mode setup
static constexpr uint8_t UBX_CFG_PRT       = 0x00;  // Used to configure port specifics.
static constexpr uint8_t UBX_CFG_PWR       = 0x57;  // Put receiver in a defined power state
static constexpr uint8_t UBX_CFG_RATE      = 0x08;  // Navigation/Measurement Rate Settings.
static constexpr uint8_t UBX_CFG_RINV      = 0x34;  // Contents of Remote Inventory
static constexpr uint8_t UBX_CFG_RST       = 0x04;  // Reset Receiver / Clear Backup Data Structures.
static constexpr uint8_t UBX_CFG_RXM       = 0x11;  // RXM configuration
static constexpr uint8_t UBX_CFG_SBAS      = 0x16;  // SBAS configuration
static constexpr uint8_t UBX_CFG_TMODE3    = 0x71;  // Time Mode Settings 3. Used to enable Survey In Mode
static constexpr uint8_t UBX_CFG_TP5       = 0x31;  // Time Pulse Parameters
static constexpr uint8_t UBX_CFG_USB       = 0x1B;  // USB Configuration

// The following are used to configure INF UBX messages (information messages)
static constexpr uint8_t UBX_INF_CLASS   = 0x04;  // All INF messages have 0x04 as the class
static constexpr uint8_t UBX_INF_DEBUG   = 0x04;  // ASCII output with debug contents
static constexpr uint8_t UBX_INF_ERROR   = 0x00;  // ASCII output with error contents
static constexpr uint8_t UBX_INF_NOTICE  = 0x02;  // ASCII output with informational contents
static constexpr uint8_t UBX_INF_TEST    = 0x03;  // ASCII output with test contents
static constexpr uint8_t UBX_INF_WARNING = 0x01;  // ASCII output with warning contents

// The following are used to configure LOG UBX messages (loggings messages)
static constexpr uint8_t UBX_LOG_CREATE           = 0x07;  // Create Log File
static constexpr uint8_t UBX_LOG_ERASE            = 0x03;  // Erase Logged Data
static constexpr uint8_t UBX_LOG_FINDTIME         = 0x0E;  // Find index of a log entry based on a given time
static constexpr uint8_t UBX_LOG_INFO             = 0x08;  // Poll for log information, or Log information
static constexpr uint8_t UBX_LOG_RETRIEVEPOSEXTRA = 0x0F;  // Odometer log entry
static constexpr uint8_t UBX_LOG_RETRIEVEPOS      = 0x0B;  // Position fix log entry
static constexpr uint8_t UBX_LOG_RETRIEVESTRING   = 0x0D;  // Byte string log entry
static constexpr uint8_t UBX_LOG_RETRIEVE         = 0x09;  // Request log data
static constexpr uint8_t UBX_LOG_STRING           = 0x04;  // Store arbitrary string on on-board flash

// The following are used to configure MGA UBX messages (Multiple GNSS Assistance Messages).
static constexpr uint8_t UBX_MGA_ACK_DATA0      = 0x60;  // Multiple GNSS Acknowledge message
static constexpr uint8_t UBX_MGA_BDS_EPH        = 0x03;  // BDS Ephemeris Assistance
static constexpr uint8_t UBX_MGA_BDS_ALM        = 0x03;  // BDS Almanac Assistance
static constexpr uint8_t UBX_MGA_BDS_HEALTH     = 0x03;  // BDS Health Assistance
static constexpr uint8_t UBX_MGA_BDS_UTC        = 0x03;  // BDS UTC Assistance
static constexpr uint8_t UBX_MGA_BDS_IONO       = 0x03;  // BDS Ionospheric Assistance
static constexpr uint8_t UBX_MGA_DBD            = 0x80;  // Poll the Navigation Database
static constexpr uint8_t UBX_MGA_GAL_EPH        = 0x02;  // Galileo Ephemeris Assistance
static constexpr uint8_t UBX_MGA_GAL_ALM        = 0x02;  // Galileo Almanac Assitance
static constexpr uint8_t UBX_MGA_GAL_TIMOFFSET  = 0x02;  // Galileo GPS time offset assistance
static constexpr uint8_t UBX_MGA_GAL_UTC        = 0x02;  // Galileo UTC Assistance
static constexpr uint8_t UBX_MGA_GLO_EPH        = 0x06;  // GLONASS Ephemeris Assistance
static constexpr uint8_t UBX_MGA_GLO_ALM        = 0x06;  // GLONASS Almanac Assistance
static constexpr uint8_t UBX_MGA_GLO_TIMEOFFSET = 0x06;  // GLONASS Auxiliary Time Offset Assistance
static constexpr uint8_t UBX_MGA_GPS_EPH        = 0x00;  // GPS Ephemeris Assistance
static constexpr uint8_t UBX_MGA_GPS_ALM        = 0x00;  // GPS Almanac Assistance
static constexpr uint8_t UBX_MGA_GPS_HEALTH     = 0x00;  // GPS Health Assistance
static constexpr uint8_t UBX_MGA_GPS_UTC        = 0x00;  // GPS UTC Assistance
static constexpr uint8_t UBX_MGA_GPS_IONO       = 0x00;  // GPS Ionosphere Assistance
static constexpr uint8_t UBX_MGA_INI_POS_XYZ    = 0x40;  // Initial Position Assistance
static constexpr uint8_t UBX_MGA_INI_POS_LLH    = 0x40;  // Initial Position Assitance
static constexpr uint8_t UBX_MGA_INI_TIME_UTC   = 0x40;  // Initial Time Assistance
static constexpr uint8_t UBX_MGA_INI_TIME_GNSS  = 0x40;  // Initial Time Assistance
static constexpr uint8_t UBX_MGA_INI_CLKD       = 0x40;  // Initial Clock Drift Assitance
static constexpr uint8_t UBX_MGA_INI_FREQ       = 0x40;  // Initial Frequency Assistance
static constexpr uint8_t UBX_MGA_INI_EOP        = 0x40;  // Earth Orientation Parameters Assistance
static constexpr uint8_t UBX_MGA_QZSS_EPH       = 0x05;  // QZSS Ephemeris Assistance
static constexpr uint8_t UBX_MGA_QZSS_ALM       = 0x05;  // QZSS Almanac Assistance
static constexpr uint8_t UBX_MGA_QZAA_HEALTH    = 0x05;  // QZSS Health Assistance

// The following are used to configure the MON UBX messages (monitoring messages)
static constexpr uint8_t UBX_MON_COMMS = 0x36;  // Comm port information
static constexpr uint8_t UBX_MON_GNSS  = 0x28;  // Information message major GNSS selection
static constexpr uint8_t UBX_MON_HW2   = 0x0B;  // Extended Hardware Status
static constexpr uint8_t UBX_MON_HW3   = 0x37;  // HW I/O pin information
static constexpr uint8_t UBX_MON_HW    = 0x09;  // Hardware Status
static constexpr uint8_t UBX_MON_IO    = 0x02;  // I/O Subsystem Status
static constexpr uint8_t UBX_MON_MSGPP = 0x06;  // Message Parse and Process Status
static constexpr uint8_t UBX_MON_PATCH = 0x27;  // Output information about installed patches
static constexpr uint8_t UBX_MON_RF    = 0x38;  // RF information
static constexpr uint8_t UBX_MON_RXBUF = 0x07;  // Receiver Buffer Status
static constexpr uint8_t UBX_MON_RXR   = 0x21;  // Receiver Status Information
static constexpr uint8_t UBX_MON_TXBUF = 0x08;  // Transmitter Buffer Status. Used for query tx buffer size/state.
static constexpr uint8_t UBX_MON_VER   = 0x04;  // Receiver/Software Version. Used for obtaining Protocol Version.

// The following are used to configure the NAV UBX messages (navigation results messages)
static constexpr uint8_t UBX_NAV_ATT       = 0x05;  // Vehicle "Attitude" Solution
static constexpr uint8_t UBX_NAV_CLOCK     = 0x22;  // Clock Solution
static constexpr uint8_t UBX_NAV_DOP       = 0x04;  // Dilution of precision
static constexpr uint8_t UBX_NAV_EOE       = 0x61;  // End of Epoch
static constexpr uint8_t UBX_NAV_GEOFENCE  = 0x39;  // Geofencing status. Used to poll the geofence status
static constexpr uint8_t UBX_NAV_HPPOSECEF = 0x13;  // High Precision Position Solution in ECEF
static constexpr uint8_t UBX_NAV_HPPOSLLH  = 0x14;  // High Precision Geodetic Position Solution.
static constexpr uint8_t UBX_NAV_ODO       = 0x09;  // Odometer Solution
static constexpr uint8_t UBX_NAV_ORB       = 0x34;  // GNSS Orbit Database Info
static constexpr uint8_t UBX_NAV_POSECEF   = 0x01;  // Position Solution in ECEF
static constexpr uint8_t UBX_NAV_POSLLH    = 0x02;  // Geodetic Position Solution
static constexpr uint8_t UBX_NAV_PVT       = 0x07;  // Navigation Position Velocity Time Solution.
static constexpr uint8_t UBX_NAV_RELPOSNED = 0x3C;  // Relative Positioning Information in NED frame
static constexpr uint8_t UBX_NAV_RESETODO  = 0x10;  // Reset odometer
static constexpr uint8_t UBX_NAV_SAT       = 0x35;  // Satellite Information
static constexpr uint8_t UBX_NAV_SIG       = 0x43;  // Signal Information
static constexpr uint8_t UBX_NAV_STATUS    = 0x03;  // Receiver Navigation Status
static constexpr uint8_t UBX_NAV_SVIN      = 0x3B;  // Survey-in data. Used for checking Survey In status
static constexpr uint8_t UBX_NAV_TIMEBDS   = 0x24;  // BDS Time Solution
static constexpr uint8_t UBX_NAV_TIMEGAL   = 0x25;  // Galileo Time Solution
static constexpr uint8_t UBX_NAV_TIMEGLO   = 0x23;  // GLO Time Solution
static constexpr uint8_t UBX_NAV_TIMEGPS   = 0x20;  // GPS Time Solution
static constexpr uint8_t UBX_NAV_TIMELS    = 0x26;  // Leap second event information
static constexpr uint8_t UBX_NAV_TIMEUTC   = 0x21;  // UTC Time Solution
static constexpr uint8_t UBX_NAV_VELECEF   = 0x11;  // Velocity Solution in ECEF
static constexpr uint8_t UBX_NAV_VELNED    = 0x12;  // Velocity Solution in NED

// The following are used to configure the RXM UBX messages
static constexpr uint8_t UBX_RXM_MEASX = 0x14;  // Satellite Measurements for RRLP
static constexpr uint8_t UBX_RXM_PMREQ = 0x41;  // Requests a Power Management task (two differenent packet sizes)
static constexpr uint8_t UBX_RXM_RAWX  = 0x15;  // Multi-GNSS Raw Measurement Data
static constexpr uint8_t UBX_RXM_RLM   = 0x59;  // Galileo SAR Short-RLM report (two different packet sizes)
static constexpr uint8_t UBX_RXM_RTCM  = 0x32;  // RTCM input status
static constexpr uint8_t UBX_RXM_SFRBX = 0x13;  // Boradcast Navigation Data Subframe

// ACK ID constants
static constexpr uint8_t UBX_ACK_NACK = 0x00;
static constexpr uint8_t UBX_ACK_ACK  = 0x01;

// Port types and ID's
static constexpr uint8_t COM_PORT_I2C   = 0;
static constexpr uint8_t COM_PORT_UART1 = 1;
static constexpr uint8_t COM_PORT_UART2 = 2;
static constexpr uint8_t COM_PORT_USB   = 3;
static constexpr uint8_t COM_PORT_SPI   = 4;

static constexpr uint8_t COM_TYPE_UBX   = (1 << 0);
static constexpr uint8_t COM_TYPE_NMEA  = (1 << 1);
static constexpr uint8_t COM_TYPE_RTCM3 = (1 << 5);

typedef struct {
  uint8_t cls;
  uint8_t id;
  uint16_t len;           // Length of the payload. Does not include cls, id, or checksum bytes
  uint16_t counter;       // Keeps track of number of overall bytes received. Some responses are larger than 255 bytes.
  uint16_t startingSpot;  // The counter value needed to go past before we begin recording into payload array
  uint8_t *payload;       // array containing payload data
  uint8_t checksumA;      // Given to us from module. Checked against the rolling calculated A/B checksums.
  uint8_t checksumB;
  ublox_packet_validity_e valid;  // Goes from NOT_DEFINED to VALID or NOT_VALID when checksum is checked
  // Goes from NOT_DEFINED to VALID or NOT_VALID when the Class and ID match the requestedClass and requestedID
  ublox_packet_validity_e classAndIDmatch;
} ubxPacket;

// Depending on the sentence type the processor will load characters into different arrays
enum SentenceTypes { NONE = 0, NMEA, UBX, RTCM } currentSentence = NONE;

class GPS {
 public:
  GPS(PinName tx, PinName rx, uint32_t baud_rate);
  void configure();                 // configures the GPS (note that serial loop must be started before this is run)
  void saveConfiguration();         // saves configuration to persistent memory
  void process_byte_from_buffer();  // processes next byte from circular buffer
  double getLat();                  // degrees
  double getLong();                 // degrees
  double getAlt();                  // meters above elipsoid
  double getTime();                 // seconds since midnight

 private:
  // configuration functions
  void setPortOutput(uint8_t portID, uint8_t comSettings);
  void setPortInput(uint8_t portID, uint8_t comSettings);
  void getPortSettings(uint8_t portID);
  void setSerialRate(uint32_t baudrate, uint8_t uartPort = COM_PORT_UART1);
  void configureMessage(uint8_t msgClass, uint8_t msgID, uint8_t portID, uint8_t sendRate);
  void setNavigationFrequency(uint8_t navFreq);

  // data processing functions

  void processNonPayload(uint8_t incoming, ubxPacket *incomingUBX);
  void processPayload(uint8_t incoming, ubxPacket *incomingUBX);
  void extractPacketData(ubxPacket *msg);
  void selectProcessBuffer(uint8_t incoming, ubxPacket *incomingUBX);
  void handleACK(uint8_t incoming);
  void validateChecksum(ubxPacket *incomingUBX);

  void calcChecksum(ubxPacket *msg);
  void addToChecksum(uint8_t incoming);  // Given an incoming byte, adjust rollingChecksumA/B

  // serial functions
  void sendSerialCommand(ubxPacket *outgoingUBX);
  void onByteReceive();
  bool waitForAck(uint8_t msgClass, uint8_t msgID);

  // bit manipulation functions
  uint32_t extractLong(uint8_t spotToStart);  // Combine four bytes from payload into long
  uint16_t extractInt(uint8_t spotToStart);   // Combine two bytes from payload into int
  uint8_t extractByte(uint8_t spotToStart);   // Get byte from payload

  // payload arrays
  uint8_t payloadAck[2]{};                 // Holds the requested ACK/NACK
  uint8_t payloadCfg[MAX_PAYLOAD_SIZE]{};  // Holds the requested data packet
  uint8_t payloadBuf[2]{};  // Temporary buffer used to screen incoming packets or dump unrequested packets

  // packet structures (for processing)
  ubxPacket m_packetAck = {
      0, 0, 0, 0, 0, payloadAck, 0, 0, UBLOX_PACKET_VALIDITY_NOT_DEFINED, UBLOX_PACKET_VALIDITY_NOT_DEFINED};
  ubxPacket m_packetCfg = {
      0, 0, 0, 0, 0, payloadCfg, 0, 0, UBLOX_PACKET_VALIDITY_NOT_DEFINED, UBLOX_PACKET_VALIDITY_NOT_DEFINED};
  ubxPacket m_packetBuf = {
      0, 0, 0, 0, 0, payloadBuf, 0, 0, UBLOX_PACKET_VALIDITY_NOT_DEFINED, UBLOX_PACKET_VALIDITY_NOT_DEFINED};

  // helper variables for processing functions
  uint8_t m_rollingChecksumA;  // Rolls forward as we receive incoming bytes
  uint8_t m_rollingChecksumB;  // Rolls forward as we receive incoming bytes
  uint16_t m_ubxFrameCounter;
  ublox_packet_buffer_e m_activePacketBuffer = UBLOX_PACKET_PACKETBUF;
  bool m_ignoreThisPayload;

  // navigation data struct
  // TODO: check if each member needs to be volatile, or just the struct itself
  volatile struct {
    int16_t year;
    int8_t month;
    int8_t day;
    int8_t hour;
    int8_t minute;
    int8_t second;
    int32_t nanosecond;       // in ns, includes all sub-second data
    int32_t latitude;         // Degrees * 10^-7
    int32_t longitude;        // Degrees * 10^-7
    int32_t altitude;         // Number of mm above ellipsoid
    int32_t groundSpeed;      // mm/s
    int32_t headingOfMotion;  // Degrees * 10^-5
  } m_nav_data{};

  // ack signaling flags
  volatile bool m_ack_flag;
  volatile bool m_nack_flag;

  // class and msg ID targets
  volatile uint8_t m_target_class;
  volatile uint8_t m_target_msgID;

  // waitForAck timeout timer
  Timer m_ack_timeout;

  // serial input buffer
  array<uint8_t, BUFFER_SIZE> m_buffer;
  array<uint8_t, BUFFER_SIZE>::iterator m_buf_write;
  array<uint8_t, BUFFER_SIZE>::iterator m_buf_read;

  // serial interface
  Serial m_uart;
};
