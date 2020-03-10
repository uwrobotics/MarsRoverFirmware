#include "mbed.h"
#include "blockingneopixel.h"
#include "PinNames.h"

int main(){
        BlockingNeopixel pixels(16, LED_MTRX);

        while (1)       
        {
            pixels.displayRed();
            wait(1);
            pixels.displayBlue();
            wait(1);
        }
        
    return 1;
}
