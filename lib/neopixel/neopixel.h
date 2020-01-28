#include "mbed.h"

/*
    Data is written in G -> R -> B
    Each byte is a number value from 0-255 of the intensity of that individual LED light
*/

class neopixel
{
private:
    DigitalOut out(PC_8);
    int pixelNum;
    int on_buffer[8] = {1,1,1,1,1,1,1,1};
    int off_buffer[8] = {0};
    
public:
    neopixel(/* args */);
    ~neopixel();
    void pulse_1();
    void pulse_0();
    void writeByte(const int buffer[8]);
    void showColour(char colour);
    void blinkPixels(int flashes, char colour);
    void writeAnyRGB(const int colour[3]);

    void displayRed();
    void displayBlue();
    void flashGreen(int numFlashes, int delay_s);
};


