#pragma once

#include "mbed.h"

class Encoder {

public:

	typedef enum {
		relative,
		absolute
	} t_encoderType;

	typedef struct {
		t_encoderType encoderType;
		PinName encoderPins[3];
	} t_encoderConfig;

	Encoder(t_encoderConfig config) {};
	virtual ~Encoder() {};

	// Must be implemented
	virtual t_encoderConfig getType() = 0;
	virtual float getAngle_Degrees() = 0;
	virtual float getVelocity_DegreesPerSec() = 0;
	virtual float getRevolutions() = 0;

	virtual mbed_error_status_t reset();

};