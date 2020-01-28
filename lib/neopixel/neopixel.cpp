#include "neopixel.h"

#define H_1_NS 400
#define L_1_NS 100
#define H_0_NS 150
#define L_0_NS 300

neopixel::neopixel(DigitalOut pin, int numPixels)
{
    pixelNum = numPixels;
    out(pin);
}

neopixel::~neopixel()
{
}

/*
    Writes 1 to the neopixels
    delay is based on the NRZ timing
*/
void neopixel::pulse_1() {
    out.write(1);
    wait_ns(H_1_NS);
    out.write(0);
    wait_ns(L_1_NS);
}

/*
    Writes 0 to the neopixels
    delay is based on the NRZ timing
*/
void neopixel::pulse_0(){
    out.write(1);
    wait_ns(H_0_NS);
    out.write(0);
    wait_ns(L_0_NS);
}

/*
Writes a byte of data to the pixels. Pixels require 3 bytes of data to be sent
*/
void neopixel::writeByte(const int buffer[8])
{
    for (int i = 7; i >= 0; i--)
    {
        if(buffer[i] == 1)
        //while bit value is 1
            pulse_1(out);
        else
            pulse_0(out);   
    }
}

/* 
    Writes byte data needed for the specified colour character.
    g = green
    b = blue
    r = red
    w = white
    n = no colour/off
*/
void neopixel::showColour(char colour)
{
    switch (colour)
    {
    case 'g':
        writeByte(out, on_buffer);
        writeByte(out, off_buffer);
        writeByte(out, off_buffer);
        break;
    case 'b':
        writeByte(out, off_buffer);
        writeByte(out, off_buffer);
        writeByte(out, on_buffer);
        break;
    case 'r':
        writeByte(out, off_buffer);    
        writeByte(out, on_buffer);
        writeByte(out, off_buffer);
        break;
    case 'w':
        writeByte(out, on_buffer);
        writeByte(out, on_buffer);
        writeByte(out, on_buffer);
        break;
    case 'n':
        writeByte(out, off_buffer);    
        writeByte(out, off_buffer);
        writeByte(out, off_buffer);
        break;

    // can possibly add more colours but will need custom byte data so send        
    default:
        break;
    }
}

//tells pixels to turn on and off 3 times
void neopixel::blinkPixels(int flashes, char colour)
{
    for (int i = 0; i < flashes; i++)
    {
        for (int i = 0; i < pixelNum i++)
        {
            showColour(colour);       
        }    
        wait(0.5);
    }
}

void neopixel::writeAnyRGB(const int colour[3])
{
    int colourBuffer[3] = {colour[0], colour[1], colour[2]};
    int buffer[3][8];
    for(int j = 0; j< 3; j++)
    {
        for(int i = 0; i < 8; i++){
            buffer[j][i] = colourBuffer[j]%2;
            colourBuffer[j] /= 2;
        }
    }
    writeByte(out, buffer[0]);
    writeByte(out, buffer[1]);    
    writeByte(out, buffer[2]);
}

void neopixel::flashGreen(int numFlashes, int delay_s)
{
    for (int i = 0; i < numFlashes; i++)
    {
        for (int i = 0; i < pixelNum; i++)
        {
            showColour('g');
        }
        wait(delay_s);

        for (int i = 0; i < pixelNum; i++)
        {
            showColour('n');
        }
        wait(delay_s);
    }
}

void neopixel::displayRed()
{
    for (int i = 0; i < pixelNum; i++)
    {
        showColour('r');
    }
}

void neopixel::displayBlue()
{
    for (int i = 0; i < pixelNum; i++)
    {
        showColour('b');
    }
}