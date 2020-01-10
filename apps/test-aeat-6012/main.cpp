#include "mbed.h"

#define READ PC_0
#define CLOCK PC_1
#define CONTROL PC_2

DigitalOut cs(CONTROL);

uint16_t pos = 0;

int main(){
/* ISSUE: ONLY OUTPUTTING 7 BITS"

POTENTIAL FIX: ENSURING CLOCK SIGNAL IS AT LEAST 3.5 V

POTENTIAL FIX: CHECK BIT SHFITING RELATED STUFF

*/

//    SPI enc(CONTROL, READ, CLOCK);
    SPI enc(SPI_MOSI, SPI_MISO, SPI_SCK);
    enc.format(12,3);
    enc.frequency(1000000);

    wait_ms(100);

    printf("connected");
    while(1){
        cs = 0;

        //write low for wait bit
        wait_ns(500);

        //write 12 low dummy bits to recieve
        pos = enc.write(0X000);

        //bring CONTROL back to HIGH to stop signaling
        cs = 1;

        printf("%d \r\n", pos);

        wait_ms(100);
    }
}