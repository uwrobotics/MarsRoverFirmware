#include "mbed.h"
#include "CANMsg.h"
#include "INA_226.h"
#include "Voltage_Divider_Thermistor.h"
#include "rover_config.h"

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf
// current sensor data https://docs.google.com/spreadsheets/d/1Qn4QbEJ1Ia54vYnTgaTvLxHralQvCz6PEGaSF_O1L20/edit#gid=0

//Serial communications for debugging
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

//CAN initialization 
CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY); //TODO: dont know the actual CAN id's, find out can id for safety
CANMsg rxMsg;
//TODO: create possible user commands
constexpr unsigned int SET_ALERT_LIMIT = 0x00;
constexpr unsigned int SET_CONFIGURATIONS = 0x01;

//measurement thresholds, needs updating, comparison in celcius 
constexpr float CURRENT_UPPER_LIM = 1;
constexpr float CURRENT_LOWER_LIM = 0;
constexpr float CURVOLTAGE_UPPER_LIM = 1;
constexpr float VOLTAGE_LOWER_LIM = 0;
constexpr float TEMP_UPPER_LIM = 1;
constexpr float TEMP_LOWER_LIM = 0;  

constexpr int NUM_SAFETY_SENSORS = 6;
enum SENSORNAMES{
    elbow_sensor,
    claw_sensor,
    wrist_sensor,
    bicep_sensor,
    allen_key_sensor,
    turntable_sensor
};

void initCAN();
//TODO may need more functions to deal with different CAN message contents
void validateCANMsg(CANMsg *p_newMsg);
bool inRange(float upper_lim, float lower_lim, float val);

int main()
{
    // SDA Pin, SCL pin, sensor address, shunt resistance, max expected current
    //TODO: confirm values for sensors
    ComponentConfig elbow_config = {CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.003, 28.75};
    ComponentConfig claw_config = {CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL,0x00, 0.012, 6.325};
    ComponentConfig wrist_config = {CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL,0x00, 0.01, 8.05};
    ComponentConfig bicep_config = {CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL,0x00, 0.015, 5.75};
    ComponentConfig allen_key_config = {CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL,0x00, 0.022, 3.45};
    ComponentConfig turntable_config = {CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL,0x00, 0.00075, 60.95};

    INA_226 safety_sensors[] = {
        INA_226 (elbow_config), 
        INA_226 (claw_config),
        INA_226 (wrist_config),
        INA_226 (bicep_config),
        INA_226 (allen_key_config),
        INA_226 (turntable_config)
    };

    //thermistor pin, B constant, vin, expected room temp, thermistor room temp resistance, voltage divider resistance
    VoltageDividerThermistorConfig safety_thermistor_config = {TEMP_ANLG_IN, 4700, 5, 295.15, 100000, 100000};
    VoltageDividerThermistor temp_sensor(safety_thermistor_config); 

    initCAN();

    safety_sensors[elbow_sensor].calibrateSensor(); //replace index with enum

    while (1) {
        //read can messages to get data to configure device
        u_int8_t command = 0;
        if(can.read(rxMsg))
        {
            validateCANMsg(&rxMsg);
            rxMsg >> command;
            rxMsg.clear();
            // if (command == mask_enable)
            // {
            //     elbow_current_sensor.setMaskEnableRegister();            }
            // }
            // else if(command == configure_sensor)
            // {
            //     elbow_current_sensor.configureSensor(command);  
            // }
        }

        //TODO: implement hardware indication of danger
        for(int sensor_index = 0; sensor_index < NUM_SAFETY_SENSORS; sensor_index++)
        {
            float measured_current = safety_sensors[sensor_index].getCurrentData();
            if (!inRange(CURRENT_UPPER_LIM, CURRENT_LOWER_LIM, measured_current))
            {
                printf("Measured Current is NOT within expected limits!\r\n");
            }

            float measured_voltage = safety_sensors[sensor_index].getVoltageData();
            if (!inRange(CURVOLTAGE_UPPER_LIM, VOLTAGE_LOWER_LIM, measured_voltage))
            {
                printf("Measured Voltage is NOT within expected limits!\r\n");
            }

            float measured_temp = temp_sensor.readThermistorCelcius();
            if(!inRange(TEMP_UPPER_LIM, TEMP_LOWER_LIM, measured_temp))
            {
                printf("Measured Temperature is NOT within expected limits!\r\n");
            }
        }
    }
    return 1;
}

void initCAN()
{
    // will need new id's 
    can.filter(ROVER_CANID_FIRST_SAFETY_RX, ROVER_CANID_FILTER_MASK, CANStandard);
}

//may change function name to reflect the processing of the can message when functionality to manipulate INA is made
void validateCANMsg(CANMsg *p_newMsg)
{
    // PRINT_INFO("Recieved CAN message with ID %X\r\n", p_newMsg->id);
    switch (p_newMsg->id){
        case SET_CONFIGURATIONS:
            printf("Updating INA226 config\r\n");
            //TODO: do stuff to update INA config register
            break;
        case SET_ALERT_LIMIT:
            printf("Updating Alert Limits\r\n");
            //TODO: do stuff to update alert limit register
            break;
        default:
            printf("Recieved unimplemented command\r\n");
            break;
    }
}

//if in range returns 1, otherwise returns 0
bool inRange(float upper_lim, float lower_lim, float val)
{
    return (val > upper_lim || val< lower_lim) ? 0: 1;
}

