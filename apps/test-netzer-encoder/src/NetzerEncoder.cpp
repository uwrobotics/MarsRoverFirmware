#include "NetzerEncoder.h"
#include "mbed.h"

NetzerEncoder::NetzerEncoder(int frequency) : freq(frequency){}

void NetzerEncoder::spi_callback_debug(const int events){
	printf("First buffer: %u  Second buffer: %u ", rx_buffer[0], rx_buffer[0]);
}

void NetzerEncoder::spi_read(){
	
	// Callback function to be executed during reads
	const event_callback_t spi_read_callback = NetzerEncoder::spi_callback_debug;		

	spi.format(16,3);				// May have to be 8 bits?
	spi.frequency(freq);

	ThisThread::sleep_for(3ms);		// Delay

	// Read from spi constantly
	while (true){
		spi.transfer(tx_buffer, WORDS, rx_buffer, WORDS, spi_read_callback, NULL);
	}

}




