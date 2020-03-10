#include "FSR.h"

FSR::FSR(PinName pin): input(pin){
}

float FSR::getValue(){
	return input.read();
}