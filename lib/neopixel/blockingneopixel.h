#ifndef BLOCKINGNEOPIXEL_H
#define BLOCKINGNEOPIXEL_H

/*
    Data is written in G -> R -> B
    Each byte is a number value from 0-255 of the intensity of that individual LED light
*/

class BlockingNeopixel
{
private:
    int m_pixelNum;
    int m_on_buffer[8] = {1,1,1,1,1,1,1,1};
    int m_off_buffer[8] = {0};
    enum colour {Red, Blue, Green, Off, White};
public:
    BlockingNeopixel(int numPixels);
    ~BlockingNeopixel();
    void pulse_1();
    void pulse_0();
    void writeByte(const int buffer[8]);
    void showColour(colour selectedColour);
    void blinkPixels(int flashes, colour selectedColour);
    void writeAnyRGB(const int colour[3]);

    //basic functions for autonomy
    void displayRed();
    void displayBlue();
    void flashGreen(int numFlashes, float delay_s);
    void shutdown();
};

#endif /* BLOCKINGNEOPIXEL_H */
