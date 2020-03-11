#include "FSR.h"

FSR::FSR(PinName pin): input(pin){
}

float FSR::get_raw_ADC(){
	return input.read();
}
float FSR::get_force_newtons(){
	return -1.0; //TODO determine values for this conversion
}