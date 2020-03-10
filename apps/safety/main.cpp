#include "mbed.h"

//#define GPIO_TEST
#define ADC_TEST
// #define I2C_TEST_SAFETY
// #define I2C_TEST_NUCLEO
// #define CAN_TEST

#ifdef GPIO_TEST
DigitalOut led1(PC_0);
DigitalOut led2(PC_1);
DigitalIn but1(PB_0);
DigitalIn but2(PB_1);
Serial pc(PC_10, PC_11); 

int main(){
	while(true){
		if(!but1.read())
			led1 = 1;
		else if(!but2.read())
			led2 = 1;
		else{
			led1 = 0;
			led2 = 0;
		} 
	}
}

#elif defined(ADC_TEST)
DigitalOut led(PC_0);
AnalogIn   FIVE_V_BUS(PA_1);
AnalogIn   TWELVE_V_BUS(PA_2);
AnalogIn   TWENTY_FOUR_V_BUS(PA_3);
AnalogIn   TEMP(PA_4);
Serial pc(PC_10, PC_11); 

const int k_interval_ms = 500;

int main()
{
    int i = 0;

    while (true) {
        pc.printf("%f %f %f %f\r\n", FIVE_V_BUS.read(), 
TWELVE_V_BUS.read(), TWENTY_FOUR_V_BUS.read(), TEMP.read());
        i++;
        wait(0.5);
        led = i % 2;
    }
}

#elif defined(I2C_TEST_SAFETY)
I2C i2c(PB_9, PB_8); 
Serial pc(PC_10, PC_11); 
DigitalOut led1(PC_0);
DigitalOut led2(PC_1);
DigitalOut led3(PC_2);
DigitalOut led4(PC_3);
 
const int addr7bit = 0x48;      // 7 bit I2C address
const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90

int main() {
    char cmd[1] = {0XFF};
    while (1) {
       
        if (i2c.read(addr8bit, cmd, 1)){
        	pc.printf("read failure!\r\n");
        }
 		int num = cmd[0];
 		pc.printf("%d \r\n", num);
 		led1 = led2 = led3 = led4 = 1;
        if (num == 1)
        	led1 = 0;
        else if(num == 2)
        	led2 = 0;
        else if(num == 3)
        	led3 = 0;
        else if(num == 4)
        	led4 = 0;
        wait(0.5);

    }
}

#elif defined(I2C_TEST_NUCLEO)
I2CSlave  slave(PB_9, PB_8); 
DigitalOut led(LED1);
const int addr7bit = 0x48;      // 7 bit I2C address
const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90

int main(){
	char msg[4][1] = {{1}, {2}, {3}, {4}};
	int idx = 0;
	slave.address(addr8bit);

	 while (1) {
       int i = slave.receive();
       switch (i) {
           case I2CSlave::ReadAddressed:
              if(slave.write(msg[idx], 1)){
              		led = 1;
              } 
               break;
       }

       idx = (idx+1) % 4;

       wait(0.5);
       led = 0;
   }
}

#elif defined(CAN_TEST)

#include "CANMsg.h"

CAN                 can(PB_12, PB_13);
CANMsg              rxMsg;
CANMsg              txMsg;
DigitalOut          ledTX(PC_0);
DigitalOut          ledRX(PC_1);
Timer               timer;
Serial 				pc(PC_10, PC_11); 
uint8_t             counter = 0;

#define TX_ID 0x100
#define RX_ID 0x101
 
/**
 * @brief   Prints CAN msg to PC's serial terminal
 * @note
 * @param   CANMessage to print
 * @retval
 */
void printMsg(CANMessage& msg) {
    pc.printf("  ID      = 0x%.3x\r\n", msg.id);
    pc.printf("  Type    = %d\r\n", msg.type);
    pc.printf("  Format  = %d\r\n", msg.format);
    pc.printf("  Length  = %d\r\n", msg.len);
    pc.printf("  Data    =");
    for(int i = 0; i < msg.len; i++)
        pc.printf(" 0x%.2X", msg.data[i]);
    pc.printf("\r\n");
}
 
int main(void)
{
    can.frequency(500000);              // set bit rate to 50kbps
    ledTX = 0;                          // set transmit LED off
    ledRX = 0;                          // set recieve LED off
    timer.start();                      // start timer
    pc.printf("CAN_Hello\r\n");

    while(1) {
        if(timer.read_ms() >= 1000) {   // check for timeout
            timer.reset();              // reset timer
            counter++;                  // increment counter
            txMsg.clear();              // clear Tx message storage
            txMsg.id = TX_ID;           // set ID
            txMsg << counter;  // copy counter value to CAN msg payload
            if(can.write(txMsg)) {      // transmit message
                pc.printf("-------------------------------------\r\n");
                pc.printf("CAN message sent\r\n");
                printMsg(txMsg);
                pc.printf("  counter = %d\r\n", counter);
                ledTX = !ledTX;
            }
            else
                pc.printf("Transmission error\r\n");
        }
 
        if(can.read(rxMsg)) {
            ledRX = !ledRX;       // turn the LED on
            pc.printf("-------------------------------------\r\n");
            pc.printf("CAN message received\r\n");
            printMsg(rxMsg);
 
            // Filtering performed by software:
            if(rxMsg.id == RX_ID) {
                rxMsg >> counter;    // extract data from the received CAN message
                pc.printf("  counter = %d\r\n", counter);
                timer.start();                  // transmission lag
            }
        }
    }
}

#endif