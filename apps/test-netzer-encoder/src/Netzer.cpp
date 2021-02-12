#include "Netzer.h"

using namespace Encoder;
Netzer::Netzer(PinName mosi, PinName miso, PinName sclk, 
									callback_ptr callback) 
									: m_spi(mosi, miso, sclk), m_callback(callback){
	m_spi.format(16,3);
	m_spi.frequency(DEFAULT_FREQUENCY_HZ);
	m_spi.set_dma_usage(DMA_USAGE_ALWAYS);
}

bool Netzer::spi_read(){

	ThisThread::sleep_for(3ms);		// Delay

	// return status
	int status = m_spi.transfer(tx_buffer, WORDS, rx_buffer, WORDS, event_callback_t(this, &Netzer::Netzer::spi_callback_debug));
	
	// status = 0 => SPI transfer started
  // status = -1 => SPI peripheral is busy
	return (status == 0);
}

uint16_t Netzer::get_raw_data(){
	return m_raw_data;
}

void Netzer::spi_callback_debug(int events){
	// upper lower?
	const uint8_t crcPolynomial = 0x03;

	// Not sure if I'm handling the payload correctly
	// If I get the raw_data correctly, the rest should work fine (taken from Alex's test encoder)
  uint16_t raw_data = (static_cast<uint16_t>(rx_buffer[0]) << 8) | (static_cast<uint16_t>(rx_buffer[1]));

	uint32_t msg = (raw_data >> 2) & 0x0FFFFFFF;
	uint16_t ack = (msg >> 27) & 1;
  uint16_t start = (msg >> 26) & 1;
  uint16_t zero = (msg >> 25) & 1;
  uint32_t pos = (msg >> 8) & 0x1FFFF;
	uint16_t error = (msg >> 7) & 1; // 1 = good
  uint16_t warn = (msg >> 6) & 1; // 1 = good
	uint16_t crc = msg & 0x3F;
  uint32_t rem = (msg & 0x1FFFFFF) % crcPolynomial;

	// Output values
	 printf("Encoder reading raw: %d\r\n", raw_data);
  printf("Ack: %d \t Start: %d \t Zero: %d \t Pos: %d \t Error: %d \t Warn: %d \t CRC: %d \t Rem: %d \n",
           ack, start, zero, (int) pos, error, warn, crc, (int) rem);
  printf("Pos: 0x%08X \t CRC: 0x%04X \t Msg: 0x%08X \n\n", (unsigned int) pos, crc, (unsigned int) msg);

	m_callback();
}




