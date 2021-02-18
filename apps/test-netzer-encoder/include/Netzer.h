#pragma once

#include "mbed.h"

// TODO: Integrate Encoder.h 

namespace Encoder {

class Netzer {
	typedef void (*callback_ptr)(void);

	public:
		Netzer(PinName mosi, PinName miso, PinName sclk, callback_ptr callback);
		bool spi_read();
		uint16_t get_raw_data();
		
		// Encoder api (TODO: after inheriting, override)
		bool getAngleDeg(float &angle);
		bool getAngularVelocity(float &speed);
		bool reset();
		
		//Add get_angular_vel and get_angle functions
	
	private:
		static constexpr uint32_t DEFAULT_FREQUENCY_HZ = 20000;  // frequency given by datasheet
		static const int WORDS = 4;											// 2, 8 bit words
		const uint8_t tx_buffer[WORDS] = {0};						// Initialize dummy tx
		uint8_t rx_buffer[WORDS] = {0};									// Initialize rx buffer to receive data (16 bits in total)
		uint16_t m_raw_data;														// between 14-19 bits
		SPI m_spi;																				// most, miso, sclk (PA_7, PA_6, PA_5)
  	callback_ptr m_callback;
		void spi_callback_debug(int events);
		// User callback function to be invoked when an encoder read is complete (only for async reads)
};

}


