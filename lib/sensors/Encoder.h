#pragma once

#include "mbed.h"
template <class t_encoderConfig>
class Encoder {

public:

	typedef enum {
		relative,
		absolute
	} t_encoderType;

	Encoder(t_encoderConfig config);
	virtual ~Encoder();

	// Must be implemented
	virtual t_encoderConfig getType() = 0;
	virtual float getAngle_Degrees() = 0;
	virtual float getVelocity_DegreesPerSec() = 0;
	virtual float getRevolutions() = 0;

	virtual mbed_error_status_t reset();

private:



};