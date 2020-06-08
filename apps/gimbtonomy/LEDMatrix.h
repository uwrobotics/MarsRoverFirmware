/* LED Matrix Header */

#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <mbed.h>
#include <CANMsg.h>

//Use whatever uint8_t red, green, blue gets assigned by the CAN messages to set the PWM of R, G, B 

/*typedef union {
	uint32_t  hexColourCode;
		struct {
			uint8_t green;
			uint8_t blue;
			uint8_t red;
		} coloursRGB;
} HexColours;*/

class LEDMatrix {
	
	public:
		//Data constructor - pins determined beforehand
		//LEDMatrix();
		
		void setColour(uint8_t msgRed, uint8_t msgGreen, uint8_t msgBlue);

		void setFrequency(uint8_t msgFrequency);

		void setColourFrequency(uint8_t red, uint8_t green, uint8_t blue, uint8_t frequency);
		
	private: 
		//PWM object for each colour. Each connected to a PWM pin
		PwmOut redPWM;
		PwmOut greenPWM;
		PwmOut bluePWM;
		
		uint8_t setDutyCycle(uint8_t msgColour);

		//Use what red, green, blue gets assigned by the CAN messages to set  PWM of R, G, B objects
		//CAN can1(CAN_RX, CAN_TX, 500000);
			
};



#endif
