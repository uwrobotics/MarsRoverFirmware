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
  uint16_t raw_data = (static_cast<uint16_t>(rx_buffer[1]) << 8) | (static_cast<uint16_t>(rx_buffer[0]));
	m_raw_data = raw_data;
	//TODO: 
	// Actually get the position data from this (look at datasheet)
	// ack, start,  zero, pos, error, warn, crc, rem
	m_callback();
}




