#pragma once

#include "mbed.h"

/*
Class to read analog FSR values and convert them to digital ones
*/

class FSR{
public:
	FSR(PinName pin);

	float getValue();
	//returns a value from 1 to 0
	//0 is lowest force, 1 is highest


private:
	AnalogIn input;

	//float convertForce();
	//converts reading to force value
	//need data to implement
};