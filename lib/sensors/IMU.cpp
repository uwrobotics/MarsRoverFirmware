#include "IMU.h"
#include "IMU_util.h"

std::array<double> IMU::get_IMU_lin_accel(void) {

    update_AGM();

    std::array<double, 3> lin_accel = {0.0, 0.0, 0.0};
    double sensitivity_factor;

    // get gyro sensitivity scale factor
    switch (agm.fss.a) {
        case 0:
            sensitivity_factor = ACCEL_SENSITIVITY_FACTOR_0;
            break;
        case 1:
            sensitivity_factor = ACCEL_SENSITIVITY_FACTOR_1;
            break;
        case 2:
            sensitivity_factor = ACCEL_SENSITIVITY_FACTOR_2;
            break;
        case 3:
            sensitivity_factor = ACCEL_SENSITIVITY_FACTOR_3;
            break;
        default:
            sensitivity_factor = 1.0;
            break;
    }

    // raw accel data is in g's
    // to convert raw accel data to accel measurement, divide by accel sensitivity factor
    lin_accel[0] = ((double) agm.acc.x) * ACCELERATION_GRAVITY / sensitivity_factor;
    lin_accel[1] = ((double) agm.acc.y) * ACCELERATION_GRAVITY / sensitivity_factor;
    lin_accel[2] = ((double) agm.acc.z) * ACCELERATION_GRAVITY / sensitivity_factor;
    
    return lin_accel;
}

std::array<double> IMU::get_IMU_ang_vel(void) {

    update_AGM();

    std::array<double, 3> ang_vel = {0.0, 0.0, 0.0};
    double sensitivity_factor;

    // get gyro sensitivity scale factor
    switch (agm.fss.g) {
        case 0:
            sensitivity_factor = GYRO_SENSITIVITY_FACTOR_0;
            break;
        case 1:
            sensitivity_factor = GYRO_SENSITIVITY_FACTOR_1;
            break;
        case 2:
            sensitivity_factor = GYRO_SENSITIVITY_FACTOR_2;
            break;
        case 3:
            sensitivity_factor = GYRO_SENSITIVITY_FACTOR_3;
            break;
        default:
            sensitivity_factor = 1.0;
            break;
    }

    // raw gyro data is in degrees per second
    // to convert raw gyro data to gyro measurement, divide by gyro sensitivity factor
    ang_vel[0] = ((double) agm.gyr.x) / sensitivity_factor * PI / 180;
    ang_vel[1] = ((double) agm.gyr.y) / sensitivity_factor * PI / 180;
    ang_vel[2] = ((double) agm.gyr.z) / sensitivity_factor * PI / 180;
    
    return ang_vel;
}

std::array<double, 3> get_IMU_mag_field(void) {

    update_AGM();

    std::array<double, 3> mag_field = {0.0, 0.0, 0.0};

    // raw mag data is in micro-Teslas
    // to convert raw mag data to mag measurement, multiply by mag sensitivity factor
    mag_field[0] = ((double) agm.mag.x) * MAG_SENSITIVITY_FACTOR;
    mag_field[1] = ((double) agm.mag.y) * MAG_SENSITIVITY_FACTOR;
    mag_field[2] = ((double) agm.mag.z) * MAG_SENSITIVITY_FACTOR;
    
    return mag_field;
}

Status_e IMU::update_AGM(void) {
    Status_e retval = Status_Ok;

    // 6 bytes of accel + 6 bytes of gyro + 2 bytes of temp + 9 bytes of mag
    const uint8_t num_bytes = 6 + 6 + 2 + 9;  
    uint8_t buffer[num_bytes];

    // get readings
    retval |= set_bank(0);  // user bank 0
    retval |= read_register((uint8_t)AGB0_REG_ACCEL_XOUT_H, buffer, num_bytes);

    /* 
     * buffer should now hold:
     *
     * accel x (2 bytes), accel y (2 bytes), accel z (2 bytes), 
     * gyro x (2 bytes), gyro y (2 bytes), gyro z (2 bytes),
     * temp (2 bytes), 
     * mag whoami (1 byte),
     * mag status 1 (1 byte), 
     * mag x (2 bytes), mag y (2 bytes), mag z (2 bytes),
     * mag status 2 (1 byte)
     */

    // update agm values with buffer data

    agm.acc.x = ((buffer[0] << 8) | (buffer[1]) & 0xFF));
    agm.acc.y = ((buffer[2] << 8) | (buffer[3]) & 0xFF));
    agm.acc.z = ((buffer[4] << 8) | (buffer[5]) & 0xFF));

    agm.gyr.x = ((buffer[6] << 8) | (buffer[7]) & 0xFF));
    agm.gyr.y = ((buffer[8] << 8) | (buffer[9]) & 0xFF));
    agm.gyr.z = ((buffer[10] << 8) | (buffer[11]) & 0xFF));

    // disregard temperature data and mag whoami

    agm.mag_stat_1 = buffer[15];

    // mag data is read little endian
    agm.mag.x = ((buffer[17] << 8) | (buffer[16] & 0xFF));
    agm.mag.y = ((buffer[19] << 8) | (buffer[18] & 0xFF));
    agm.mag.z = ((buffer[21] << 8) | (buffer[20] & 0xFF));

    agm.mag_stat_2 = buffer[22];


    // read full scale settings (to be able to compute scaled values)
    retval |= set_bank(2); // user bank 2
    ICM_20948_ACCEL_CONFIG_t acfg;
    retval |= read_register((uint8_t)AGB2_REG_ACCEL_CONFIG, (uint8_t*)&acfg, sizeof(acfg));
    agm.fss.a = acfg.ACCEL_FS_SEL;  // Sparkfun notes:
                                    // Worth noting that without explicitly setting the FS range of the accelerometer it was showing the register value for +/- 2g but the reported values were actually scaled to the +/- 16g range
									// Wait a minute... now it seems like this problem actually comes from the digital low-pass filter. When enabled the value is 1/8 what it should be...
    retval |= set_bank(2);
    ICM_20948_GYRO_CONFIG_1_t gcfg1;
    retval |= read_register((uint8_t)AGB2_REG_GYRO_CONFIG_1, (uint8_t*)&gcfg1, sizeof(gcfg1));
    agm.fss.g = gcfg1.GYRO_FS_SEL;

    // TODO: What to do with this value??
    ICM_20948_ACCEL_CONFIG_2_t acfg2;
    retval |= read_register((uint8_t)AGB2_REG_ACCEL_CONFIG_2, (uint8_t*)&acfg2, sizeof(acfg2));

    return retval;
}

Status_e IMU::read_register(uint8_t regaddr, uint8_t *pdata, uint32_t len) {

    if (spi == NULL) {
        return Status_ParamErr;
    }

    cs = 0; // select chip
    spi.write(regaddr); // choose register to read from
    for (uint32_t i = 0; i < len; ++i) {
        *(pdata+i) = spi.write(0x00); // read data
    }
    cs = 1; // deselect chip

    return Status_Ok;
}

Status_e IMU::write_register(uint8_t regaddr, uint8_t *pdata, uint32_t len) {

    if (spi == NULL) {
        return Status_ParamErr;
    }

    cs = 0; // select chip
    spi.write(regaddr); // choose register to write to
    for (uin32_t i = 0; i < len; ++i) {
        spi.write(*(data+i)); // transmit data
    }
    cs = 1;    //deselect chip

    return Status_Ok;
}

Status_e IMU::set_bank(uint8_t bank) {

    // bank can only be 0, 1, 2, or 3
    if (bank > 3) {
        return Status_ParamErr;
    }
    bank = (bank << 4) & 0x30; // bits 5:4 of REG_BANK_SEL
    write_register(REG_BANK_SEL, &bank, 1);
}

Status_e IMU::init_SPI(PinName mosi_pin, PinName miso_pin, PinName sclk_pin, PinName cs_pin, uint8_t SPI_freq) {
    
    // configure SPI pins
    spi = SPI(mosi_pin, miso_pin, sclk_pin);
    cs(cs_pin); 

    cs = 0; // deselect device
    // TODO
    //spi.format(8, 3) // set transmission format CHECK THIS!!!
    (SPI_freq > MAX_SPI_FREQ) ? spi.frequency(MAX_SPI_FREQ) : spi.frequency(SPI_freq); // set SPI frequency
    
    cs = 1; // select device

    // initialize IMU
    Status_e retval = init_IMU();
    return retval;
}

Status_e IMU::init_IMU(void) {
    Status_e retval = Status_Ok;

    // check that device whoami matches whoami of ICM-20948
    retval = check_ID();
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    // perform a software reset
    retval = sw_reset();
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    // delay a bit
    ThisThread::sleep_for(50);

    // wake up imu
    retval = sleep(false);
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    // turn off lower power mode
    retval = low_power(false);
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    // set mode to continuous (other option is duty cycled)
    // TODO: set sample mode

    // TODO: set full scale mode
    
    // TODO: set DLFP

    // set up magnetometer
    retval = init_mag();
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    return status;
}

Status_e IMU::check_ID(void) {
    Status_e retval = Status_Ok;
    uint8_t whoami = 0x00;
    retval = get_whoami(&whoami);

    if (retval != Status_Ok) {
        return retval;
    }
    if (whoami != ICM_20948_WHOAMI) {
        return Status_WrongID;
    }
    return retval;
}

Status_e IMU::get_whoami(uint8_t *whoami) {
    if (whoami == NULL) {
        return Status_ParamErr;
    }
    set_bank(0); // user bank 0
    return read_register(AGB0_REG_WHO_AM_I, whoami, 1);
}

Status_e IMU::sw_reset() {
    Status_e retval = Status_Ok;
    ICM_20948_PWR_MGMT_1_t reg;

    set_bank(0); // user bank 0
    retval = read_register(AGB0_REG_PWR_MGMT_1, (uint8_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    if (retval != Status_Ok) {
        return retval;
    }

    reg.DEVICE_RESET = 1;

    retval = write_register(AGB0_REG_PWR_MGMT_1, (uint8_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    return retval;
}

Status_e IMU::sleep(bool on) {
    Status_e retval = Status_Ok;
    ICM_20948_PWR_MGMT_1_t reg;

    set_bank(0); // user bank 0

    retval = read_register(AGB0_REG_PWR_MGMT_1, (uint8_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    if (retval != Status_Ok) {
        return retval;
    }

    reg.SLEEP = on;

    retval = write_register(AGB0_REG_PWR_MGMT_1, (uint8_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    return retval;
}

Status_e IMU::low_power(bool on) {
    Status_e retval = Status_Ok;
    ICM_20948_PWR_MGMT_1_t reg;

    set_bank(0); // user bank 0

    retval = read_register(AGB0_REG_PWR_MGMT_1, (uint9_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    if (retval != Status_Ok) {
        return retval;
    }

    reg.LP_EN = on;

    retval = write_register(AGB0_REG_PWR_MGMT_1, (uint8_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    return retval;
}

Status_e IMU::init_mag(void) {
    Status_e retval = Status_Ok;

    // TODO
    i2c_master_enable(true);

    // after an ICM reset, the mag sensor may stop responding over the I2C master
    // reset the master I2C until it responds
    uint8_t max_tries = 5;
    uint8_t tries = 0;
    for (; tries < max_tries; ++tries) {
        retval = mag_whoami();

        // whoami matched
        if (revtal == Status_Ok) {
            break;
        }

        // TODO
        // otherwise, reset master i2c and try again
        i2c_master_reset();
    }

    if (tries == max_tries) {
        status = Status_WrongID;
        return status;
    }

    // TODO
    // set up magnetometer
    AK09916_CNTL2_Reg_t reg;
    reg.MODE = AK09916_mode_cont_100hz;
    retval = writeMag(AK09916_REG_CNTL2, (uint8_t *)&reg);
    if (retval != ICM_20948_Stat_Ok)
    {
        status = retval;
        return status;
    }

    retval = i2cMasterConfigureSlave(0, MAG_AK09916_I2C_ADDR, AK09916_REG_ST1, 9, true, true, false, false, false);
    if (retval != ICM_20948_Stat_Ok)
    {
        status = retval;
        return status;
    }

    return status;
}

Status_e IMU::is_connected(void) {
    status = check_ID();
    return status == Status_Ok;
}

Status_e IMU::data_ready(void) {
    Status_e retval = Status_Ok;

    retval = set_bank(0); // user bank 0
    if (retval != Status_Ok) {
        return retval;
    }

    ICM_20948_INT_STATUS_1_t reg;
    read_register(AGB0_REG_INT_STATUS_1, (uint8_t*)&reg, sizeof(ICM_20948_INT_STATUS_1_t));
    if (retval != Status_Ok) {
        return retval;
    }
    if (!reg.RAW_DATA_0_RDY_INT) {
        retval = Status_NoData;
    }

    return retval;
}

Status_e i2c_master_enable(bool enable) {
    Status_e retval = Status_Ok;

    // disable BYPASS_EN
    retval = i2c_master_passthrough(false);
    if (retval != Status_Ok) {
        return retval;
    }

    ICM_20948_I2C_MST_CTRL_t ctrl;
	retval = set_bank(3); // user bank 3
	if (retval != Status_Ok) {
		return retval;
	}
}

Status_e IMU::i2c_master_passthrough(bool passthrough) {
    Status_e retval = Status_Ok;

    ICM_20948_INT_PIN_CFG_t reg;
    retval = set_bank(0); // user bank 0
    if (retval != Status_Ok) {
        return retval;
    }

    retval = read_register(AGB0_REG_INT_PIN_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_INT_PIN_CFG_t));
    if (retval != Status_Ok) {
        return retval;
    }

    reg.BYPASS_EN = passthrough;
    retval = write_register(AGB0_REG_INT_PIN_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_INT_PIN_CFG_t));
    return retval;
}

const char* IMU::status_string(Status_e stat = Status_NUM) {
    
    Status_e val;
    val = (stat == Status_NUM) ? status : stat;

    switch (val) {
        case Status_Ok:
            return "All is well";
            break;
        case Status_Err:
            return "General error";
            break;
        case Status_NotImpl:
            return "Not implemented";
            break;
        case Status_ParamErr:
            return "Parameter error";
            break;
        case Status_WrongID:
            return "Wrong ID";
            break;
        case Status_InvalSensor:
            return "Invalid sensor";
            break;
        case Status_NoData:
            return "Data underflow";
            break;
        case Status_SensorNotSupported:
            return "Sensor not supported";
            break;
        default:
            return "Unknown status";
            break;
    }
    return "None";
}