#pragma once

typedef enum sensor_status_t
{
  ERR,
  NORMAL  
};

class Sensor{
  public:
	//possible input pin general variables to include?
	//
	//need to create a struct to pass into constructor?
	Sensor() = default;
	virtual ~Sensor();

	//what to do with sensors with multiple different reads(moisture/temp sensor)
	virtual float primaryRead() = 0;
	
	//some sensors may have multiple read values, optionally implemented
	virtual float alternateRead();

	//function to check on the sensor status
	virtual sensor_status_t getSensorStatus(); 

	//function to reset sensor in case of unexpected behaviour
	virtual sensor_status_t resetSensor(); 

};
