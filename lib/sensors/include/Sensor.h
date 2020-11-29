#pragma once

class Sensor{
  public:
	//possible input pin general variables to include?
	
	//need to create a struct to pass into constructor?
	Sensor() = default;
	virtual ~Sensor(){ };

	//main read function, must be implemented
	virtual float read() = 0;
	
	//some sensors may have multiple read values, optionally implemented
	virtual float alternateRead();

	//function to check on the sensor status
	virtual bool getStatus(); 

	//function to reset sensor in case of unexpected behaviour
	virtual void reset(); 
};
