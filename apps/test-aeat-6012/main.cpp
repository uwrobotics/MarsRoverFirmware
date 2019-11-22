#include "mbed.h"

#define READ PC_0
#define CLOCK PC_1
#define CONTROL PC_2

DigitalOut cs(CONTROL);

int pos = 1;

int main(){

    SPI enc(CONTROL, READ, CLOCK);
    enc.format(12,3);
    enc.frequency(1000000);


    while(1){
        cs = 0;

        //write low for wait bit
        //enc.write(0b0);

        //write 12 low dummy bits to recieve
        //pos = enc.write(0X000);

        //bring CONTROL back to HIGH to stop signaling
        //cs = 1;

        printf("%d", pos);

        wait_ms(1000);
    }
}