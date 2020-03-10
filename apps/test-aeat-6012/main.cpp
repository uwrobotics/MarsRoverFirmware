#include "mbed.h"

DigitalOut cs(SPI1_MOSI);

uint16_t pos = 0;

int main(){
/* ISSUE: ONLY OUTPUTTING 7 BITS"

POTENTIAL FIX: ENSURING CLOCK SIGNAL IS AT LEAST 3.5 V

POTENTIAL FIX: CHECK BIT SHFITING RELATED STUFF

*/

//    SPI enc(CONTROL, READ, CLOCK);
    SPI enc(NC, SPI1_MISO, SPI1_SCK);
    enc.format(12,3);
    enc.frequency(1000);

    wait_ms(100);

    printf("connected");

    char send[3] = {0x00, 0x00};
    char recieve[3] = {0x00, 0x00};

    while(1){
        cs = 0;

        //write low for wait bit
        wait_ns(500);

        //write 12 low dummy bits to recieve
        pos = enc.write(send, 2, recieve, 2);

        //bring CONTROL back to HIGH to stop signaling
        cs = 1;

        // MAX FC(LSB) 7F(MSB)    MIN 0400

        uint16_t value = (((uint16_t) recieve[1]) >> 3) | (((uint16_t) recieve[0]) << 5); 

        printf("%02X %02X %d\r\n", recieve[1], recieve[0], value);

        wait_ms(100);
    }
}