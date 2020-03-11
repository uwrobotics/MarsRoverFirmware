#include "mbed.h"

DigitalOut cs(SPI1_MOSI);
SPI enc(NC, SPI1_MISO, SPI1_SCK);

int main(){

    enc.format(12,3);
    enc.frequency(100000);

    wait_ms(100);

    printf("connected");

    char send[2] = {0x00, 0x00};
    char recieve[2] = {0x00, 0x00};

    uint16_t value = 0;

    while(1) {

        // Assert CS to start signaling
        cs = 0;

        // Specced delay >500ns after CS assertion
        ThisThread::sleep_for(1);

        // Write low dummy bytes to recieve
        pos = enc.write(send, 2, recieve, 2);

        // TODO: Next step to use async SPI

        // Deassert CS to stop signaling
        cs = 1;

        // MAX 7F(MSB) FC(LSB) , MIN 0004

        value = (((uint16_t) recieve[1]) >> 3) | (((uint16_t) recieve[0]) << 5); 

        printf("%02X %02X %d\r\n", recieve[0], recieve[1], value);

        // Specced delay >500ns after CS deassertion
        ThisThread::sleep_for(1);
    }
}