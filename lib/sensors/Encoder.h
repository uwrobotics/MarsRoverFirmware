#pragma once

#include "mbed.h"

class Encoder {

public:

	template <class t_encoderConfig>;

	typedef enum t_encoderType {
		relative,
		absolute
	}

	typedef enum t_encoderMode {
		active,	// Constantly updating internal class members
		passive // Only updates on read call
	}

	virtual Encoder(t_encoderConfig config);
	virtual ~Encoder();

	virtual t_encoderConfig getType() = 0;
	virtual t_encoderMode getMode() = 0;
	virtual float getAngle_Degrees() = 0;
	virtual float getVelocity_DegreesPerSec() = 0;
	virtual float getRevolutions() = 0;

	virtual mbed_error_status_t reset();

private:



}