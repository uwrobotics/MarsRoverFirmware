/* LED Matrix Source File */

#include "LEDMatrix.h"

// Constructor: An LED Matrix is defined by the pins it is connected to
LEDMatrix :: LEDMatrix(PinName R, PinName G, PinName B)
    : _RChannel(R), _GChannel(G), _BChannel(B)
{}

// This function takes in values from 0 to 255 for each color stream and maps them to a PWM signal
LEDMatrix :: void setColor(uint8_t R, uint8_t G, uint8_t B)
{
    _RChannel.pulsewidth((1.0/255.0)*R);
    _GChannel.pulsewidth((1.0/255.0)*G);
    _BChannel.pulsewidth((1.0/255.0)*B);
}

// This function is just for convenience. It simply calls the above function. For example if RED is passed in, call the above function with setColor(255, 0, 0)
LEDMatrix :: void setColor(color c)
{
    switch(c)
    {
        case color::RED:
            setColor(255, 0, 0);
            break;
        case color::GREEN:
            setColor(0, 255, 0);
            break;
        case color::BLUE:
            setColor(0, 0, 255);
            break;
    }
}

// This function should continously set the specified color, wait some time, turn off all LEDs, wait some time and repeat until another function is called by the main.cpp
LEDMatrix :: void setFlashingColor(uint8_t R, uint8_t G, uint8_t B, uint8_t frequency_hz)
{
    float flash_num = frequency_hz*HW_BRIDGE::LEDMATRIX::NUM_FLASHES; // Needs to flash for 10sec
    float timeout_ms = (1.0/frequency_hz)*1000;
    for (uint8_t i=0; i<flash_num; i++)
    {
        SetColour(R, G, B);
        std::ThisThread::sleep_for(std::chrono::milliseconds(timeout_ms/2)); //div by 2 cuz theres 2 timeouts in each loop
        clear();
        std::ThisThread::sleep_for(std::chrono::milliseconds(timeout_ms/2));
    }
}

// This function is just for convenience. It simply calls the above function. For example if RED is passed in, call the above function with setColor(255, 0, 0)
LEDMatrix :: void setFlashingColor(color c, uint8_t frequency_hz)
{
    switch(c)
    {
        case color::RED:
            setFlashingColor(255, 0, 0, frequency_hz);
            break;
        case color::GREEN:
            setFlashingColor(0, 255, 0, frequency_hz);
            break;
        case color::BLUE:
            setFlashingColor(0, 0, 255, frequency_hz);
            break;
    }    
}

// This function should turn off all the LEDs. This should just call setColor(0,0,0) I think (double check this)
LEDMatrix :: void clear()
{
    setColour(0, 0, 0);
}

// This is just for convenience - MOVE TO HW_BRIDGE
/*
enum class color 
{
    RED, GREEN, BLUE
};
*/