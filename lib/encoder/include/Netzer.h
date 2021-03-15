#pragma once

#include <mutex>
#include "Encoder.h"

/*
 *  Netzer DS-25 Datasheet:
 * 	https://netzerprecision.com/wp-content/uploads/2020/12/DS-25-V02-1.pdf
 */ 

namespace Encoder {

class Netzer : public Encoder {

	public:
		
		typedef void (*callback_ptr)(void);
		Netzer(PinName mosi, PinName miso, PinName sclk, callback_ptr callback, float offset_deg);
		uint16_t get_raw_data();
		
		// Blocking Reads
		bool getAngleDeg(float &angle) override;
		bool getAngularVelocityDegPerSec(float &speed) override;

		bool readAsync(callback_ptr callback);

		// Return stored values (for use after calling readAsync)
		float getAngleDegAsync(void);
		float getAngularVelocityDegPerSecAsync(void);

		// Estimator function
		void angularVelocityEstimation(void);

		// Reset encoder values and offset
		bool reset() override;
	
	private:
		static constexpr uint32_t DEFAULT_FREQUENCY_HZ = 20000;  // frequency given by datasheet
		static constexpr float FLOAT_COMPARE_TOLERANCE      = 1e-6;
  	static constexpr float MOVING_AVERAGE_FILTER_WEIGHT = 0.8;

		static const int WORDS = 4;											// 4 * 8 bit words = 32 bits
		const char tx_buffer[WORDS] = {0};						// Initialize dummy tx
		char rx_buffer[WORDS] = {0};									// Initialize rx buffer to receive data (16 bits in total)
		uint32_t m_raw_data;														// 32 bit raw data

		float m_position_deg;
  	float m_angular_velocity_deg_per_sec;
  	uint16_t m_position_raw;
  	float m_offset_deg;
  	// User callback function to be invoked when an encoder read is complete (only for async reads)
		callback_ptr m_callback;
		
		SPI m_spi;																				// most, miso, sclk (PA_7, PA_6, PA_5)

		void spi_callback_debug(int events);
		
		// Trigger a blocking encoder read
  	bool read(void);

		float rawAbsToDegrees(uint16_t raw);

    Timer m_timer;
		Mutex m_mutex;
};

}


