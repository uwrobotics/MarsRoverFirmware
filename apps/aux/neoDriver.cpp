#include "neoDriver.h"

unsigned char* toSend;

static void ok(int a){
	return;
}

const event_callback_t cb = callback(ok);

NeoDriver::NeoDriver(): NeoPixelSPI(PinName(NEO_MOSI), PinName(NEO_MISO), PinName(NEO_SCLK), PinName(NEO_SSEL)){
	
	NeoPixelSPI.format(8,0);
	NeoPixelSPI.frequency(2400000);

}

//Converts one byte of data into a horrible format for bitbashing
//1 becomes 110 and 0 becomes 100
unsigned long int convertToThree(unsigned char in){
	unsigned long tr = 0;
	for(int i = 0;i < 8;i++){
		tr>>3;
		if(in%2 == 1){
			tr+=0xC00000;
		}else{
			tr+=0x800000;
		}
		in = in<<1;
	}
	return tr;
}


int NeoDriver::displayNP(unsigned char* data, int data_length){
	
	toSend = new unsigned char[data_length*3 + 90];
	unsigned long int temp = 0;
	
	for(int i = 0;i < data_length;i++){
		temp = convertToThree(data[i]);
		toSend[i*3] = temp%255;
		temp = temp<<8;
		toSend[i*3 + 1] = temp%255;
		temp = temp<<8;
		toSend[i*3 + 2] = temp%255;
	}
	
	for(int i = 0;i < 90;i++){
		toSend[i + data_length*3] = 0;
	}

	const unsigned char* sending = toSend;

	unsigned char *np = {0};

	NeoPixelSPI.lock();
	int ret = NeoPixelSPI.transfer(sending, data_length*3 + 90, np, 0, cb);
	NeoPixelSPI.unlock();
	
	
	return ret - (data_length * 3 + 90);
	
	//sNeoPixelSPI.transfer(data, data_length, NULL, 0, NULL, NULL);
}

