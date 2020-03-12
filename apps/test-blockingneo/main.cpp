#include "mbed.h"
#include "blockingneopixel.h"

int main(){
        BlockingNeopixel pixels(16);

        while (1)       
        {
            pixels.displayRed();
            wait(1);
            pixels.displayBlue();
            wait(1);
            pixels.flashGreen();
        }
        
    return 1;
}
