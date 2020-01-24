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
		float degreesPerUnit;
	} t_encoderConfig;

	Encoder(t_encoderConfig encoderConfig) {};
	virtual ~Encoder() {};

	// Must be implemented
	virtual t_encoderType getType() = 0;
	virtual float getAngle_Degrees() = 0;
	virtual float getVelocity_DegreesPerSec() = 0;
	
	// Optionally implemented
	virtual float getRevolutions();
	virtual mbed_error_status_t reset();

};