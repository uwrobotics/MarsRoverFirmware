#include "mbed.h"
#include "neoPixel.cpp"
/*
    Developing 2 different NeoPixel drivers - Blocking & Non-Blocking drivers

    Blocking Driver:
    Uses a digital output to directly drive NeoPixel array. MCU manually pulses all bits to drive NeoPixel.


    Non-Blocking Driver:
    Uses a timer circuit to pulse binary to NeoPixel daisy chain. Can only support individual primary colours (Red, Green, Blue).

    The driver is considered non-blocking as a (digital) output controlling the timer circuit only requires input by controller every
    8 bits sent to the NeoPixel.

    Time to set colour of 1 pixel: 2(0.6) us/bit * 24 bits/pixel = 28.8us
    Time to set array of X pixels: Number of pixels * 28.8us
    Max Clock Frequency for Cortex M4 (also Max CPU cycles/s): 240e6 Hz

    => Controller can execute up to ~2300 cycles before needing to modify control on timer circuit.
    Alternatively, a "blocking" driver could execute up to ~280 cycles before needing to return to a NeoPixel task.

    Technically, an external circuit is not necessary if internal PWM peripheral timing accuracy is sufficient to drive NeoPixels.
*/

class NeoPixel{
    private:
        const uint8_t
        colors[3][3] = {(0,255,0),(255,0,0),(0,0,255)}; // 0 = R, 1 = G, 2 = B

        const int // protocol pulse timing
        H_1_ns = 600,
        L_1_ns = 600,
        H_0_ns = 300,
        L_0_ns = 900,

        flash_ms = 500;

        bool buffer [8];
        bool blocking;

        enum color{Red, Green, Blue};
        enum state{steady, blink};

    public:
        //Constructors
        NeoPixel(DigitalOut out, bool blocking);

        //Functions
        void reset();

        void show(enum color, enum state);

        void fill_buffer(bool *buffer);
        void pulse_1();
        void pulse_0();
}