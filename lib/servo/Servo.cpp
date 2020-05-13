#include "Servo.h"

Servo::Servo(PinName pin): m_pin(pin), m_pwm(pin) {}   

Servo::~Servo(){}

/* all these methods from the base Servo class returns false or -1, while the
derviced Servo class will overwrite the ones corresponding to them
*/
bool Servo::setRange(float range){
	return false;
}   

bool Servo::setMaxSpeed(float max_speed){
	return false;
} 

float Servo::getRange(void){
	return -1;
}     

float Servo::getMaxSpeed(void){
	return -1;
}     

bool Servo::setPosition(float angle){
	return false;
} 

bool Servo::setSpeed(float speed){
	return false;
}    

float Servo::read(void){
	return -1;
}  

void Servo::setPeriod(int period){
	m_pwm.period_us(period*1000);
}