#include "FSR.h"

FSR::FSR(PinName pin): input(pin){
}

float FSR::getValue(){
	return 1 - input.read();
}