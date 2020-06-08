/* LED Matrix Source File */

#include "LEDMatrix.h"


//Data constructor
//LEDMatrix :: LEDMatrix (){
    //initialize pins for PWM - pins tbd
    //return;
    //redPWM(/*redLED*/);
    //greenPWM(/*greenLED*/);
    //bluePWM(/*blueLED*/);}

//Set how often it flashes/sec
void LEDMatrix :: setFrequency(uint8_t msgFrequency)
{
    redPWM.period(1/msgFrequency);
    greenPWM.period(1/msgFrequency);
    bluePWM.period(1/msgFrequency);
}

//Set brightness/amount (pulsewidth) of each colour
uint8_t LEDMatrix :: setDutyCycle(uint8_t msgColour)
{
    return (1/255)*msgColour;
}

void LEDMatrix :: setColour(uint8_t msgRed, uint8_t msgGreen, uint8_t msgBlue) //HEX
{
    //msgColour.getPayload(colours);

    redPWM.pulsewidth(setDutyCycle(msgRed));
    greenPWM.pulsewidth(setDutyCycle(msgGreen));
    bluePWM.pulsewidth(setDutyCycle(msgBlue));
}

void LEDMatrix :: setColourFrequency(uint8_t red, uint8_t green, uint8_t blue, uint8_t frequency)
{
    setColour(red, green, blue);
    setFrequency(frequency);
}

//Solid red
//Solid blue
//Flashing green
