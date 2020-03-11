#pragma once

#include "mbed.h"

/*
Class to read analog FSR values and convert them to digital ones
*/

class FSR{
public:
	FSR(PinName pin);

	float get_raw_ADC();
	float get_force_newtons();
	//returns a value from 0 to 1


private:
	AnalogIn input;

	//float convertForce();
	//converts reading to force value
	//need data to implement
};