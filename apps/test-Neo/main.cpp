#include "mbed.h"

const uint8_t     
GREEN[3] = {255, 0 ,0},
RED [3] = {0, 255, 0},
BLUE[3] = {0, 0, 255};

const int PIXEL_NUM = 10;
bool red[24] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};

const int
H_1_ns = 800,
L_1_ns = 450,
H_0_ns = 400,
L_0_ns = 850;

DigitalOut out(PC_8);
void pulse_1(DigitalOut out);
void pulse_0(DigitalOut out);
void output(DigitalOut out, bool buffer[24*PIXEL_NUM]);


int main(){
    bool buffer[24*PIXEL_NUM];

    while(1){

        for(int k = 0; k < PIXEL_NUM; k++) 
        {  
                for(int j = 0; j < 3; j++){
                int color = GREEN[j];

                    for(int i = 0; i < 8; i++){
                        buffer[i+8*j+k*24] = color%2;
                        color /= 2;
                    } 
                }
        }
        output(out, buffer);

        wait_ms(500);
    }
    return 1;
}

void pulse_1(DigitalOut out) {
    out.write(1);
    wait_ns(H_1_ns);
    out.write(0);
    wait_ns(L_1_ns);
}

void pulse_0(DigitalOut out){
    out.write(1);
    wait_ns(H_0_ns);
    out.write(0);
    wait_ns(L_0_ns);
}

void output(DigitalOut out, bool buffer[24*PIXEL_NUM]){
    for(int k = 24*PIXEL_NUM-1; k >=0; k--){
        if(buffer[k] == 1)
            pulse_1(out);
        else
            pulse_0(out);
    }
}