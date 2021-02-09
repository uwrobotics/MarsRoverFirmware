#pragma once

#include "mbed.h"


namespace NetzerEncoder {

typedef void (*callback_ptr)(void);

class NetzerEncoder {

	public:
		NetzerEncoder(PinName mosi, PinName miso, PinName sclk, 
									callback_ptr callback, uint32_t frequency = DEFAULT_FREQUENCY_HZ);
		bool spi_read();
		uint16_t get_raw_data();
	
	private:
		static constexpr uint32_t DEFAULT_FREQUENCY_HZ = 500000;  // frequency given by datasheet
		static const int WORDS = 2;											// 2, 8 bit words
		const uint8_t tx_buffer[WORDS] = {0};						// Initialize dummy tx
		uint8_t rx_buffer[WORDS] = {0};									// Initialize rx buffer to receive data (16 bits in total)
		uint16_t m_raw_data;														// between 14-19 bits
		SPI spi;																				// most, miso, sclk (PA_7, PA_6, PA_5)
  	callback_ptr m_callback;
		uint32_t freq;																	// SPI read frequency
		void spi_callback_debug(int events);
		// User callback function to be invoked when an encoder read is complete (only for async reads)
};

}


