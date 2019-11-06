#include "neoDriver.h"

unsigned char* toSend;

static void ok(int a){
	return;
}

const event_callback_t cb = callback(ok);

NeoDriver::NeoDriver(): NeoPixelSPI(PinName(NEO_MOSI), PinName(NEO_MISO), PinName(NEO_SCLK), PinName(NEO_SSEL)){
	
	NeoPixelSPI.format(8,0);
	NeoPixelSPI.frequency(2900000);

}

//Converts one byte of data into a horrible format for bitbashing
//1 becomes 1100 and 0 becomes 1000
unsigned long int convertToFour(unsigned char in){
	unsigned long tr = 0;
	for(int i = 0;i < 8;i++){
		tr = tr>>4;
		if(in%2 == 1){
			tr+=0xC00000;
		}else{
			tr+=0x800000;
		}
		in = in>>1;
	}
	return tr;
}


int NeoDriver::displayNP(unsigned char* data, int data_length){
	
	toSend = new unsigned char[data_length*4 + 90];
	unsigned long int temp = 0;
	

	for(int i = 0;i < data_length;i++){

		temp = convertToFour(data[i]);
		toSend[i*4] = (unsigned char) temp%255;
		temp = temp>>8;

		toSend[i*4 + 1] = (unsigned char) temp%255;
		temp = temp>>8;

		toSend[i*4 + 2] = (unsigned char) temp%255;
		temp = temp>>8;
		
		toSend[i*4 + 3] = (unsigned char) temp%255;
	}
	
	for(int i = 0;i < 90;i++){
		toSend[i + data_length*4] = 0;
	}

	for(int i = 0;i < 90 + data_length*4;i++){
		printf("%d\n\r", toSend[i]);
	}

	unsigned char *np = {0};

	NeoPixelSPI.lock();
	//int ret = NeoPixelSPI.write(sending, data_length*3 + 90, np, 0);
	int ret = NeoPixelSPI.transfer(toSend, data_length*4 + 90, np, 0, cb);
	NeoPixelSPI.unlock();
	delete toSend;	
	
	return ret - (data_length * 4 + 90);
}

