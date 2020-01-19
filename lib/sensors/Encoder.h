#pragma once

#include "mbed.h"

class Encoder {

public:

	typedef struct {
		PinName 
	}

	typedef enum t_encoderType {
		relative,
		absolute
	} 

	Encoder(PinName pinA, t_encoderType type, bool inverted);
	Encoder(PinName pinA, PinName pinB, t_encoderType type, bool inverted);
	Encoder(PinName pinA, PinName pinB, PinName pinIDX, t_encoderType type, bool inverted);

	float getAngle();
	float getRevolutions();

	mbed_error_status_t 
	mbed_error_status_t reset();

private:



}