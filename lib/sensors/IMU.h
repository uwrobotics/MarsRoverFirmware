/* InvenSense ICM-20948 IMU Sensor Library
 *
 * Note: This implementation only supports SPI interface
 *
 * Ported from Sparkfun's Arduino library: https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary
 * Datasheet: https://invensense.tdk.com/wp-content/uploads/2016/06/DS-000189-ICM-20948-v1.3.pdf
 *
 */

#ifndef _IMU_H_
#define _IMU_H_

#include "mbed.h"
#include <array>

#define MAX_SPI_FREQ 7000000;

// Status codes
typedef enum {
    Status_Ok = 0x00,
    Status_Err,
    Status_NotImpl,       // returned by virtual functions that are not implemented
    Status_ParamErr,
    Status_WrongID,
    Status_InvalSensor,   // tried to apply a function to a sensor that does not support it
    Status_NoData,
    Status_SensorNotSupported,

    Status_NUM,
    Status_Unkonwn
} Status_e;

// Internal sensor IDs
typedef enum {
    Internal_Acc = (1 << 0),
    Internal_Gyr = (1 << 1),
    Internal_Mag = (1 << 2),
    Internal_Tmp = (1 << 3),
    Internal_Mst = (1 << 4)
} InternalSensorID_bm;

// Full-scale settings
typedef struct {
    uint8_t a : 2;
    uint8_t g : 2;
    uint8_t reserved_0 : 4;
} FSS_t;

// DLPF (digital low pass filter) settings
typedef struct {
    uint8_t a;
    uint8_t g;
} DLPF_cfg_t;

// TODO: check what smplrt stands for
typedef struct {
    uint8_t a;
    uint8_t g;
} SMPLRT_t;

// Interrupt enable
typedef struct {
    uint8_t I2C_MST_INT_EN : 1;
    uint8_t DMP_INT1_EN : 1;
    uint8_t PLL_RDY_EN : 1;
    uint8_t WOM_INT_EN : 1;
    uint8_t REG_WOF_EN : 1;
    uint8_t RAW_DATA_0_RDY_EN : 1;
    uint8_t FIFO_OVERFLOW_EN_4 : 1;
    uint8_t FIFO_OVERFLOW_EN_3 : 1;
    uint8_t FIFO_OVERFLOW_EN_2 : 1;
    uint8_t FIFO_OVERFLOW_EN_1 : 1;
    uint8_t FIFO_OVERFLOW_EN_0 : 1;
    uint8_t FIFO_WM_EN_4 : 1;
    uint8_t FIFO_WM_EN_3 : 1;
    uint8_t FIFO_WM_EN_2 : 1;
    uint8_t FIFO_WM_EN_1 : 1;
    uint8_t FIFO_WM_EN_0 : 1;
} INT_enable_t;

// Structure to store x, y, z axes
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} Axes;

// Structure to hold accel, gyro, and mag data
typedef struct {
    Axes acc;
    Axes gyr;
    Axes mag;
    uint8_t mag_stat_1;
    uint8_t mag_stat_2;
    FSS_t fss;  // full-scale range settings
} AGM_t;

// --------------------------------------------------------------------------------

class IMU {

    protected:
        Status_e read_register(uint8_t regaddr, uint8_t *pdata, uint32_t len);
        Status_e write_register(uint8_t regaddr, uint8_t *pdata, uint32_t len);

    public:

        IMU();

        // SPI
        SPI _spi;
		DigitalOut _cs;

        Status_e init_SPI(PinName mosi, PinName miso, PinName sclk, PinName cs, uint8_t SPI_freq);
		Status_e init_IMU(void);

        // Measurement data
        AGM_t agm;
        Status_e update_AGM(void);
        Status_e data_ready(void);  

        std::array<double> get_IMU_orientation(void);
        std::array<double> get_IMU_lin_vel(void);
        std::array<double> get_IMU_ang_vel();
        std::array<double> get_IMU_mag_field();

        // ID
        uint8_t get_whoami(uint8_t *whoami);
        bool is_connected(void);
		Status_e check_ID(void);

        // Magnetometer specific
        Status_e init_mag(void);
        uint8_t get_mag_whoami(void);
        uint8_t read_mag();
        Status_e write_mag();

        // Status
        Status_e status;
        const char* status_string(Status_e = Status_NUM);   // converts status to human readable string

        // Device configurations
        Status_e set_bank(uint8_t bank);
        Status_e sw_reset(void);
        Status_e sleep(bool on);
        Status_e low_power(bool on);

        // not sure if these are needed
        Status_e set_sample_mode();
        Status_e set_full_scale_mode();
        Status_e set_DLPF_cfc();
        Status_e enable_DLPF();
        Status_e set_sample_rate();
        Status_e clear_interrupts();
        Status_e cfc_int_active_low();
        Status_e cfc_int_open_drain();
        Status_e cfc_int_any_read_to_clear();
        Status_e cfc_fsync_int_mode();

        Status_e int_enable_DMP();
        Status_e int_enable_WOM();
        Status_e int_enable_raw_data_ready();
        Status_e int_enable_PLL();
        Status_e int_enable_WOF();
        Status_e int_enable_overflow_FIFO();
        Status_e int_enable_watermark_FIFO();

        Status_e set_clock_source(ICM_20948_PWR_MGMT_1_CLKSEL_e source);

        // Interface
        Status_e i2c_master_pass_through(void);
        Status_e i2c_master_enable(void);
        Status_e i2c_master_reset(void);
};

#endif