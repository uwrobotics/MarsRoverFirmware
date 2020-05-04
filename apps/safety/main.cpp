#include "mbed.h"
#include "CANMsg.h"
#include "INA_226.h"
#include "thermistor.h"
#include "rover_config.h"

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf
// current sensor data https://docs.google.com/spreadsheets/d/1Qn4QbEJ1Ia54vYnTgaTvLxHralQvCz6PEGaSF_O1L20/edit#gid=0

//Serial communications for debugging
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

//CAN initialization 
CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY); // dont know the actual CAN id's
CANMsg rxMsg;
//needs a specific CANID to update values
constexpr unsigned int EXPECTED_CAN_ID = 0x000;

//measurement thresholds, needs updating, comparison in celcius now
constexpr float CURRENT_UPPER_LIM = 1;
constexpr float CURRENT_LOWER_LIM = 0;
constexpr float CURVOLTAGE_UPPER_LIM = 1;
constexpr float VOLTAGE_LOWER_LIM = 0;
constexpr float TEMP_UPPER_LIM = 1;
constexpr float TEMP_LOWER_LIM = 0;    

//sample struct for component specific data
struct elbow_data
{
    float shunt_resistance;
    float max_expected_current;
    u_int8_t sensor_address; 
};


void initCAN();
//TODO may need more functions to deal with different CAN message contents
void validateCANMsg(CANMsg *p_newMsg);
bool inRange(float upper_lim, float lower_lim, float val);

int main()
{
    INA_226 elbow_current_sensor(); //example for elbow sensor, may initialize for each component
    thermistor temp_sensor(TEMP_ANLG_IN); 

    initCAN();

    elbow_current_sensor.calibrateSensor();

    while (true) {
        //read can messages to get data to configure device
        u_int8_t command = 0;
        if(can.read(rxMsg))
        {
            ValidateCANMsg(&rxMsg);
            rxMsg >> command;
            rxMsg.clear();
        }
        //going to need more functionality to select what to measure

        float measured_current = elbow_current_sensor.getCurrentData();
        if (!inRange(CURRENT_UPPER_LIM, CURRENT_LOWER_LIM, measured_current))
        {
            pc.printf("Measured Current is NOT within expected limits!\r\n");
        }

        float measured_voltage = elbow_current_sensor.getVoltageData();
        if (!inRange(CURVOLTAGE_UPPER_LIM, VOLTAGE_LOWER_LIM, measured_voltage))
        {
            pc.printf("Measured Voltage is NOT within expected limits!\r\n");
        }

        float measured_temp = temp_sensor.readThermistor();
        if(!inRange(TEMP_UPPER_LIM, TEMP_LOWER_LIM, measured_temp))
        {
            pc.printf("Measured Temperature is NOT within expected limits!\r\n");
        }
    }
}

void initCAN()
{
    // will need new id's 
    can.filter(CAN1_RX, CAN1_TX, ROVER_CANID_MASK, CANStandard);
}

void validateCANMsg(CANMsg *p_newMsg)
{
    // PRINT_INFO("Recieved CAN message with ID %X\r\n", p_newMsg->id);
    // The specific can ID for configuring chip is ___
    switch (p_newMsg->id){
        case EXPECTED_CAN_ID:
            pc.printf("Updating INA226 config\r\n");
            break;
        default:
            pc.printf("Recieved unimplemented command\r\n");
            break;
    }
}

//if in range returns 1, otherwise returns 0
bool inRange(float upper_lim, float lower_lim, float val)
{
    bool in_range = (val > upper_lim || val< lower_lim) ? 0: 1;
    return in_range;
}

