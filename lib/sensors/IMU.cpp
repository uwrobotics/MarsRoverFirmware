#include "IMU.h"

std::array<double, 3> IMU::get_IMU_lin_accel(void) {

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

std::array<double, 3> IMU::get_IMU_ang_vel(void) {

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

std::array<double, 3> IMU::get_IMU_mag_field(void) {

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
    retval = set_bank(0);  // user bank 0
    if (status != Status_Ok) {
        return status;
    }
    retval = read_register((uint8_t)AGB0_REG_ACCEL_XOUT_H, buffer, num_bytes);
    if (status != Status_Ok) {
        return status;
    }

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
    agm.acc.x = ((buffer[0] << 8) | (buffer[1] & 0xFF));
    agm.acc.y = ((buffer[2] << 8) | (buffer[3] & 0xFF));
    agm.acc.z = ((buffer[4] << 8) | (buffer[5] & 0xFF));

    agm.gyr.x = ((buffer[6] << 8) | (buffer[7] & 0xFF));
    agm.gyr.y = ((buffer[8] << 8) | (buffer[9] & 0xFF));
    agm.gyr.z = ((buffer[10] << 8) | (buffer[11] & 0xFF));

    // disregard temperature data and mag whoami

    agm.mag_stat_1 = buffer[15];

    // mag data is read little endian
    agm.mag.x = ((buffer[17] << 8) | (buffer[16] & 0xFF));
    agm.mag.y = ((buffer[19] << 8) | (buffer[18] & 0xFF));
    agm.mag.z = ((buffer[21] << 8) | (buffer[20] & 0xFF));

    agm.mag_stat_2 = buffer[22];


    // read full scale settings (to be able to compute scaled values)
    retval = set_bank(2); // user bank 2
    if (status != Status_Ok) {
        return status;
    }
    ICM_20948_ACCEL_CONFIG_t acfg;
    retval = read_register((uint8_t)AGB2_REG_ACCEL_CONFIG, (uint8_t*)&acfg, sizeof(acfg));
    if (status != Status_Ok) {
        return status;
    }
    agm.fss.a = acfg.ACCEL_FS_SEL;  // Sparkfun notes:
                                    // Worth noting that without explicitly setting the FS range of the accelerometer it was showing the register value for +/- 2g but the reported values were actually scaled to the +/- 16g range
									// Wait a minute... now it seems like this problem actually comes from the digital low-pass filter. When enabled the value is 1/8 what it should be...
    retval = set_bank(2);
    if (status != Status_Ok) {
        return status;
    }
    ICM_20948_GYRO_CONFIG_1_t gcfg1;
    retval = read_register((uint8_t)AGB2_REG_GYRO_CONFIG_1, (uint8_t*)&gcfg1, sizeof(gcfg1));
    if (status != Status_Ok) {
        return status;
    }
    agm.fss.g = gcfg1.GYRO_FS_SEL;

    // TODO: What to do with this value??
    ICM_20948_ACCEL_CONFIG_2_t acfg2;
    retval = read_register((uint8_t)AGB2_REG_ACCEL_CONFIG_2, (uint8_t*)&acfg2, sizeof(acfg2));
    if (status != Status_Ok) {
        return status;
    }

    return retval;
}

Status_e IMU::read_register(uint8_t regaddr, uint8_t *pdata, uint32_t len) {

    cs = 0; // select chip
    spi.write(regaddr); // choose register to read from
    for (uint32_t i = 0; i < len; ++i) {
        *(pdata+i) = spi.write(0x00); // read data
    }
    cs = 1; // deselect chip

    return Status_Ok;
}

Status_e IMU::write_register(uint8_t regaddr, uint8_t *pdata, uint32_t len) {

    cs = 0; // select chip
    spi.write(regaddr); // choose register to write to
    for (uint32_t i = 0; i < len; ++i) {
        spi.write(*(pdata+i)); // transmit data
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
    cs = DigitalOut(cs_pin); 

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

    // delay a bit CHECK THIS
    // ThisThread::sleep_for(50);

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

    // set sample mode to continuous (other option is duty cycled)
    retval = set_sample_mode((InternalSensorID_bm) (Internal_Acc | Internal_Gyr), ICM_20948_Sample_Mode_Continuous);
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    // set full scale settings
    FSS_t fss;
    fss.a = gpm2; // Can be changed; set accel range to +- 2g
    fss.g = dps250; // Can be changed; set gyro range to 250 degrees / s
    retval = set_full_scale_mode((InternalSensorID_bm) (Internal_Acc | Internal_Gyr), fss);
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    // configure DLFP
    DLPF_cfg_t dlpcfg;
    dlpcfg.a = acc_d473bw_n499bw;
    dlpcfg.g = gyr_d361bw4_n376bw5;

    retval = set_DLPF_cfc((InternalSensorID_bm) (Internal_Acc | Internal_Gyr), dlpcfg);
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    retval = enable_DLPF(Internal_Acc, false); // disable
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    retval = enable_DLPF(Internal_Gyr, false); // disable
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

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

    retval = read_register(AGB0_REG_PWR_MGMT_1, (uint8_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    if (retval != Status_Ok) {
        return retval;
    }

    reg.LP_EN = on;

    retval = write_register(AGB0_REG_PWR_MGMT_1, (uint8_t *)&reg, sizeof(ICM_20948_PWR_MGMT_1_t));
    return retval;
}

Status_e IMU::init_mag(void) {
    Status_e retval = Status_Ok;

    i2c_master_enable(true);

    // after an ICM reset, the mag sensor may stop responding over the I2C master
    // reset the master I2C until it responds
    uint8_t max_tries = 5;
    uint8_t tries = 0;
    for (; tries < max_tries; ++tries) {
        retval = check_mag_ID();

        // whoami matched
        if (retval == Status_Ok) {
            break;
        }

        // otherwise, reset master i2c and try again
        i2c_master_reset();
    }

    if (tries == max_tries) {
        status = Status_WrongID;
        return status;
    }

    // set up magnetometer
    AK09916_CNTL2_Reg_t reg;
    reg.MODE = AK09916_mode_cont_100hz; // continuous control
    retval = write_mag(AK09916_REG_CNTL2, (uint8_t *)&reg);
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    retval = i2c_master_configure_slave(0, MAG_AK09916_I2C_ADDR, AK09916_REG_ST1, 9, true, true, false, false, false);
    if (retval != Status_Ok) {
        status = retval;
        return status;
    }

    return status;
}

Status_e IMU::check_mag_ID(void) {
    Status_e retval = Status_Ok;
    
    uint8_t whoiam1, whoiam2;
    retval = read_mag(AK09916_REG_WIA1, &whoiam1);
    if (retval != Status_Ok) {
        return retval;
    }
    retval = read_mag(AK09916_REG_WIA2, &whoiam2);
    if (retval != Status_Ok) {
        return retval;
    }

    // whoami matches
    if ((whoiam1 == (MAG_AK09916_WHO_AM_I >> 8)) && (whoiam2 == (MAG_AK09916_WHO_AM_I & 0xFF))) {
        status = Status_Ok;
    }

    // whoami doesn't match
    else {
        status = Status_WrongID;
    }

    return status;
}

bool IMU::is_connected(void) {
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

Status_e IMU::set_sample_mode(InternalSensorID_bm sensors, ICM_20948_LP_CONFIG_CYCLE_e mode) {
    Status_e retval = Status_Ok;
    ICM_20948_LP_CONFIG_t reg;

    if (!(sensors & (Internal_Acc | Internal_Gyr | Internal_Mst))) {
        return Status_SensorNotSupported;
    }

    retval = set_bank(0); // user bank 0
    if (retval != Status_Ok) {
        return retval;
    }

    read_register(AGB0_REG_LP_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_LP_CONFIG_t));
    if (retval != Status_Ok) {
        return retval;
    }

    // set accel mode
    if (sensors & Internal_Acc) {
        reg.ACCEL_CYCLE = mode;
    }

    // set gyro mode
    if (sensors & Internal_Gyr) {
        reg.GYRO_CYCLE = mode;
    }

    // set master i2c mode
    if (sensors & Internal_Mst) {
        reg.I2C_MST_CYCLE;
    }

    retval = write_register(AGB0_REG_LP_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_LP_CONFIG_t));
    return retval;
}

Status_e IMU::set_full_scale_mode(InternalSensorID_bm sensors, FSS_t fss) {
    Status_e retval = Status_Ok;

    if (!(sensors & (Internal_Acc | Internal_Gyr))) {
        return Status_SensorNotSupported;
    }

    // set accel full scale settings
    if (sensors & Internal_Acc) {
        ICM_20948_ACCEL_CONFIG_t reg;
        retval = set_bank(2); // user bank 2
        if (status != Status_Ok) {
            return status;
        }
        retval = read_register(AGB2_REG_ACCEL_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_ACCEL_CONFIG_t));
        if (status != Status_Ok) {
            return status;
        }
        reg.ACCEL_FS_SEL = fss.a;
        retval = write_register(AGB2_REG_ACCEL_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_ACCEL_CONFIG_t));
        if (status != Status_Ok) {
            return status;
        }
    }

    // set gyro full scale settings
    if (sensors & Internal_Gyr) {
        ICM_20948_GYRO_CONFIG_1_t reg;
        retval = set_bank(2); //user bank 2
        if (status != Status_Ok) {
            return status;
        }
        retval = read_register(AGB2_REG_GYRO_CONFIG_1, (uint8_t *)&reg, sizeof(ICM_20948_GYRO_CONFIG_1_t));
        if (status != Status_Ok) {
            return status;
        }
        reg.GYRO_FS_SEL = fss.g;
        retval = write_register(AGB2_REG_GYRO_CONFIG_1, (uint8_t *)&reg, sizeof(ICM_20948_GYRO_CONFIG_1_t));
        if (status != Status_Ok) {
            return status;
        }
    }

    return retval;
}

Status_e IMU::set_DLPF_cfc(InternalSensorID_bm sensors, DLPF_cfg_t cfg) {
    Status_e retval = Status_Ok;

    if (!(sensors & (Internal_Acc | Internal_Gyr))) {
		return Status_SensorNotSupported;
	}

    // configure accel dlpf
    if (sensors & Internal_Acc) {
		ICM_20948_ACCEL_CONFIG_t reg;
		retval = set_bank(2); // user bank 2
        if (status != Status_Ok) {
            return status;
        }
		retval = read_register(AGB2_REG_ACCEL_CONFIG, (uint8_t *)&reg, sizeof(ICM_20948_ACCEL_CONFIG_t));
        if (status != Status_Ok) {
            return status;
        }
		reg.ACCEL_DLPFCFG = cfg.a;
		retval = write_register(AGB2_REG_ACCEL_CONFIG, (uint8_t *)&reg, sizeof(ICM_20948_ACCEL_CONFIG_t));
        if (status != Status_Ok) {
            return status;
        }
	}

    // configure gyro dlpf
	if (sensors & Internal_Gyr) {
		ICM_20948_GYRO_CONFIG_1_t reg;
		retval = set_bank(2); // user bank 2
        if (status != Status_Ok) {
            return status;
        }
		retval = read_register(AGB2_REG_GYRO_CONFIG_1, (uint8_t *)&reg, sizeof(ICM_20948_GYRO_CONFIG_1_t));
        if (status != Status_Ok) {
            return status;
        }
		reg.GYRO_DLPFCFG = cfg.g;
		retval = write_register(AGB2_REG_GYRO_CONFIG_1, (uint8_t *)&reg, sizeof(ICM_20948_GYRO_CONFIG_1_t));
        if (status != Status_Ok) {
            return status;
        }
	}

	return retval;
}

Status_e IMU::enable_DLPF(InternalSensorID_bm sensors, bool enable) {
    Status_e retval = Status_Ok;

    if (!(sensors & (Internal_Acc | Internal_Gyr))) {
		return Status_SensorNotSupported;
	}

    // enable dlpf for accel
	if (sensors & Internal_Acc) {
		ICM_20948_ACCEL_CONFIG_t reg;
		retval = set_bank(2); // user bank 2
        if (status != Status_Ok) {
            return status;
        }
		retval = read_register(AGB2_REG_ACCEL_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_ACCEL_CONFIG_t));
        if (status != Status_Ok) {
            return status;
        }
		reg.ACCEL_FCHOICE = (uint8_t) enable;
		retval = write_register(AGB2_REG_ACCEL_CONFIG, (uint8_t*)&reg, sizeof(ICM_20948_ACCEL_CONFIG_t));
        if (status != Status_Ok) {
            return status;
        }
	}

    // enable dlpf for gyro
	if (sensors & Internal_Gyr) {
		ICM_20948_GYRO_CONFIG_1_t reg;
		retval = set_bank(2); // user bank 2
        if (status != Status_Ok) {
            return status;
        }
		retval = read_register(AGB2_REG_GYRO_CONFIG_1, (uint8_t*)&reg, sizeof(ICM_20948_GYRO_CONFIG_1_t));
        if (status != Status_Ok) {
            return status;
        }
		reg.GYRO_FCHOICE = (uint8_t) enable;
		retval = write_register(AGB2_REG_GYRO_CONFIG_1, (uint8_t *)&reg, sizeof(ICM_20948_GYRO_CONFIG_1_t));
        if (status != Status_Ok) {
            return status;
        }
	}

	return retval;
}

Status_e IMU::set_sample_rate(InternalSensorID_bm sensors, SMPLRT_t smplrt) {
	Status_e retval = Status_Ok;

	if (!(sensors & (Internal_Acc | Internal_Gyr))) {
		return Status_SensorNotSupported;
	}

    // set accel sample rate
	if (sensors & Internal_Acc) {
		retval = set_bank(2); // user bank 2
        if (status != Status_Ok) {
            return status;
        }
		uint8_t div1 = (smplrt.a << 8);
		uint8_t div2 = (smplrt.a & 0xFF);
		retval = write_register(AGB2_REG_ACCEL_SMPLRT_DIV_1, &div1, 1);
        if (status != Status_Ok) {
            return status;
        }
		retval = write_register(AGB2_REG_ACCEL_SMPLRT_DIV_2, &div2, 1);
        if (status != Status_Ok) {
            return status;
        }
	}

    // set gyro sample rate
	if (sensors & Internal_Gyr) {
		retval = set_bank(2); // user bank 2
        if (status != Status_Ok) {
            return status;
        }
		uint8_t div = (smplrt.g);
		retval = write_register(AGB2_REG_GYRO_SMPLRT_DIV, &div, 1);
        if (status != Status_Ok) {
            return status;
        }
	}

	return retval;
}

Status_e IMU::i2c_master_enable(bool enable) {
    Status_e retval = Status_Ok;

    // disable BYPASS_EN
    retval = i2c_master_pass_through(false); // todo
    if (retval != Status_Ok) {
        return retval;
    }

    ICM_20948_I2C_MST_CTRL_t ctrl;
	retval = set_bank(3); // user bank 3
	if (retval != Status_Ok) {
		return retval;
	}
}

Status_e IMU::i2c_master_pass_through(bool passthrough) {
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

Status_e IMU::i2c_master_reset(void) {
    Status_e retval = Status_Ok;

	ICM_20948_USER_CTRL_t ctrl;
	retval = set_bank(0); // user bank 0
	if (retval != Status_Ok) {
		return retval;
	}

	retval = read_register(AGB0_REG_USER_CTRL, (uint8_t *)&ctrl, sizeof(ICM_20948_USER_CTRL_t));
	if (retval != Status_Ok) {
		return retval;
	}

	ctrl.I2C_MST_RST = 1; // reset

	retval = write_register(AGB0_REG_USER_CTRL, (uint8_t *)&ctrl, sizeof(ICM_20948_USER_CTRL_t));
	if (retval != Status_Ok) {
		return retval;
	}
    
	return retval;
}

Status_e IMU::read_mag(AK09916_Reg_Addr_e reg, uint8_t *pdata) {
    status = i2c_master_slv4_txn(MAG_AK09916_I2C_ADDR, reg, pdata, 1, true, true);
    return status;
} 

Status_e IMU::write_mag(AK09916_Reg_Addr_e reg, uint8_t *pdata) {
    status = i2c_master_slv4_txn(MAG_AK09916_I2C_ADDR, reg, pdata, 1, false, true);
    return status;
}

Status_e IMU::i2c_master_slv4_txn(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len, bool Rw, bool send_reg_addr) {
    Status_e retval = Status_Ok;

    addr = ((Rw) ? 0x80 : 0x00) | addr;

    // set mag i2c address
    retval = set_bank(3); //user bank 3
    if (status != Status_Ok) {
        return status;
    }
    retval = write_register(AGB3_REG_I2C_SLV4_ADDR, (uint8_t*)&addr, 1);
    if (status != Status_Ok) {
        return status;
    }
    if (retval != Status_Ok) {
        return retval;
    }

    // set mag register to read from
    retval = set_bank(3); // user bank 3
    if (retval != Status_Ok) {
        return retval;
    }
    retval = write_register(AGB3_REG_I2C_SLV4_REG, (uint8_t *)&reg, 1);
    if (retval != Status_Ok) {
        return retval;
    }

    // set control info
    ICM_20948_I2C_SLV4_CTRL_t ctrl;
	ctrl.EN = 1;
	ctrl.INT_EN = false;
	ctrl.DLY = 0;
	ctrl.REG_DIS = !send_reg_addr;

    ICM_20948_I2C_MST_STATUS_t i2c_mst_status;
	bool txn_failed = false;
	uint16_t nByte = 0;

    while (nByte < len) {

        if (!Rw) {
            retval = set_bank(3); // user bank 3
            if (retval != Status_Ok) {
                return retval;
            }
            retval = write_register(AGB3_REG_I2C_SLV4_DO, (uint8_t*)&(data[nByte]), 1);
            if (retval != Status_Ok) {
                return retval;
            }
        }

        // kick off transaction
        retval = set_bank(3); // user bank 3
        if (retval != Status_Ok) {
            return retval;
        }
        retval = write_register(AGB3_REG_I2C_SLV4_CTRL, (uint8_t*)&ctrl, sizeof(ICM_20948_I2C_SLV4_CTRL_t));
        if (retval != Status_Ok) {
            return retval;
        }

        uint32_t max_cycles = 1000;
		uint32_t count = 0;
		bool slv4_done = false;

        while (!slv4_done) {
            retval = set_bank(0);
            if (retval != Status_Ok) {
                return retval;
            }
            retval = read_register(AGB0_REG_I2C_MST_STATUS, (uint8_t*)&i2c_mst_status, 1);
            if (retval != Status_Ok) {
                return retval;
            }

            slv4_done = i2c_mst_status.I2C_SLV4_DONE;
            slv4_done |= (count >= max_cycles);
            ++count;
        }

        txn_failed = i2c_mst_status.I2C_SLV4_NACK;
        txn_failed |= (count >= max_cycles);
        if (txn_failed) {
            break;
        }

        if (Rw) {
            retval = set_bank(3);
            if (retval != Status_Ok) {
                return retval;
            }
            retval = read_register(AGB3_REG_I2C_SLV4_DI, &data[nByte], 1);
            if (retval != Status_Ok) {
                return retval;
            }
        }

        ++nByte;
    }

    if (txn_failed) {
        return Status_Err;
    }

    return retval;
}

Status_e IMU::i2c_master_configure_slave(uint8_t slave, uint8_t addr, uint8_t reg, uint8_t len, bool Rw, bool enable, bool data_only, bool grp, bool swap) {
    Status_e retval = Status_Ok;

    uint8_t slv_addr_reg;
    uint8_t slv_reg_reg;
    uint8_t slv_ctrl_reg;

    switch (slave) {
        case 0:
            slv_addr_reg = AGB3_REG_I2C_SLV0_ADDR;
            slv_reg_reg = AGB3_REG_I2C_SLV0_REG;
            slv_ctrl_reg = AGB3_REG_I2C_SLV0_CTRL;
            break;
        case 1:
            slv_addr_reg = AGB3_REG_I2C_SLV1_ADDR;
            slv_reg_reg = AGB3_REG_I2C_SLV1_REG;
            slv_ctrl_reg = AGB3_REG_I2C_SLV1_CTRL;
            break;
        case 2:
            slv_addr_reg = AGB3_REG_I2C_SLV2_ADDR;
            slv_reg_reg = AGB3_REG_I2C_SLV2_REG;
            slv_ctrl_reg = AGB3_REG_I2C_SLV2_CTRL;
            break;
        case 3:
            slv_addr_reg = AGB3_REG_I2C_SLV3_ADDR;
            slv_reg_reg = AGB3_REG_I2C_SLV3_REG;
            slv_ctrl_reg = AGB3_REG_I2C_SLV3_CTRL;
            break;
        default:
            return Status_ParamErr;
    }

    retval = set_bank(3); // user bank 3
    if (retval != Status_Ok) {
        return retval;
    }

    // set slave address and Rw flag
    ICM_20948_I2C_SLVX_ADDR_t address;
    address.ID = addr;
    if (Rw) {
        address.RNW = 1;
    }

    retval = write_register(slv_addr_reg, (uint8_t*)&address, sizeof(ICM_20948_I2C_SLVX_ADDR_t));
    if (retval != Status_Ok) {
        return retval;
    }

    // set slave sub-address (reg)
	ICM_20948_I2C_SLVX_REG_t subaddress;
	subaddress.REG = reg;
	retval = write_register(slv_reg_reg, (uint8_t *)&subaddress, sizeof(ICM_20948_I2C_SLVX_REG_t));
	if (retval != Status_Ok) {
		return retval;
	}

    // set up control info
	ICM_20948_I2C_SLVX_CTRL_t ctrl;
	ctrl.LENG = len;
	ctrl.EN = enable;
	ctrl.REG_DIS = data_only;
	ctrl.GRP = grp;
	ctrl.BYTE_SW = swap;

	retval = write_register(slv_ctrl_reg, (uint8_t *)&ctrl, sizeof(ICM_20948_I2C_SLVX_CTRL_t));
	return retval;
}

Status_e IMU::clear_interrupts(void) {
    ICM_20948_INT_STATUS_t int_stat;
    ICM_20948_INT_STATUS_1_t int_stat_1;

    // read to clear interrupts
    status = set_bank(0); // user bank 0
    if (status != Status_Ok) {
        return status;
    }

    status = read_register(AGB0_REG_INT_STATUS, (uint8_t*)&int_stat, sizeof(ICM_20948_INT_STATUS_t));
    if (status != Status_Ok) {
        return status;
    }
    status = read_register(AGB0_REG_INT_STATUS_1, (uint8_t*)&int_stat_1, sizeof(ICM_20948_INT_STATUS_1_t));
    if (status != Status_Ok) {
        return status;
    }

    // TODO: there may be additional interrupts that need to be cleared, like FIFO overflow/watermark

    return status;
}

Status_e IMU::cfg_int(ICM_20948_INT_PIN_CFG_t *write, ICM_20948_INT_PIN_CFG_t *read) {
    Status_e retval = Status_Ok;

    retval = set_bank(0); // user bank 0
    if (write) {
        retval = write_register(AGB0_REG_INT_PIN_CONFIG, (uint8_t*)write, sizeof(ICM_20948_INT_PIN_CFG_t));
        if (retval != Status_Ok) {
            return retval;
        }
    }

    if (read) {
        retval = read_register(AGB0_REG_INT_PIN_CONFIG, (uint8_t*)read, sizeof(ICM_20948_INT_PIN_CFG_t));
        if (retval != Status_Ok) {
            return retval;
        }
    }

    return retval;
}

Status_e IMU::cfg_int_active_low(bool active_low) {
    ICM_20948_INT_PIN_CFG_t reg;
    status = cfg_int(NULL, &reg); // read phase
    if (status != Status_Ok) {
        return status;
    }

    reg.INT1_ACTL = active_low;                           // set the setting
    status = cfg_int(&reg, NULL); // write phase
    return status;
}

Status_e IMU::cfg_int_open_drain(bool open_drain) {
    ICM_20948_INT_PIN_CFG_t reg;
    status = cfg_int(NULL, &reg); // read phase
    if (status != Status_Ok) {
        return status;
    }

    reg.INT1_OPEN = open_drain;                           // set the setting
    status = cfg_int(&reg, NULL); // write phase
    return status;
}

Status_e IMU::cfg_int_latch(bool latching) {
    ICM_20948_INT_PIN_CFG_t reg;
    status = cfg_int(NULL, &reg); // read phase
    if (status != Status_Ok) {
        return status;
    }

    reg.INT1_LATCH_EN = latching;                         // set the setting
    status = cfg_int(&reg, NULL); // write phase
    return status;
}

Status_e IMU::cfg_int_any_read_to_clear(bool enabled) {
    ICM_20948_INT_PIN_CFG_t reg;
    status = cfg_int(NULL, &reg); // read phase
    if (status != Status_Ok) {
        return status;
    }

    reg.INT_ANYRD_2CLEAR = enabled;                       // set the setting
    status = cfg_int(&reg, NULL); // write phase
    return status;
}

Status_e IMU::cfg_fsync_active_low(bool active_low) {
    ICM_20948_INT_PIN_CFG_t reg;
    status = cfg_int(NULL, &reg); // read phase
    if (status != Status_Ok) {
        return status;
    }
    
    reg.ACTL_FSYNC = active_low;                          // set the setting
    status = cfg_int(&reg, NULL); // write phase
    return status;
}

Status_e IMU::cfg_fsync_int_mode(bool interrupt_mode) {
    ICM_20948_INT_PIN_CFG_t reg;
    status = cfg_int(NULL, &reg); // read phase
    if (status != Status_Ok) {
        return status;
    }

    reg.FSYNC_INT_MODE_EN = interrupt_mode;               // set the setting
    status = cfg_int(&reg, NULL); // write phase
    return status;
}

Status_e IMU::int_enable(INT_enable_t *write, INT_enable_t *read) {
    Status_e retval = Status_Ok;

    ICM_20948_INT_ENABLE_t en_0;
	ICM_20948_INT_ENABLE_1_t en_1;
	ICM_20948_INT_ENABLE_2_t en_2;
	ICM_20948_INT_ENABLE_3_t en_3;

    retval = set_bank(0); // user bank 0

    // if write is not null, write to registers before reading
    if (write) {
        en_0.I2C_MST_INT_EN = write->I2C_MST_INT_EN;
		en_0.DMP_INT1_EN = write->DMP_INT1_EN;
		en_0.PLL_READY_EN = write->PLL_RDY_EN;
		en_0.WOM_INT_EN = write->WOM_INT_EN;
		en_0.REG_WOF_EN = write->REG_WOF_EN;
		en_1.RAW_DATA_0_RDY_EN = write->RAW_DATA_0_RDY_EN;
		en_2.individual.FIFO_OVERFLOW_EN_4 = write->FIFO_OVERFLOW_EN_4;
		en_2.individual.FIFO_OVERFLOW_EN_3 = write->FIFO_OVERFLOW_EN_3;
		en_2.individual.FIFO_OVERFLOW_EN_2 = write->FIFO_OVERFLOW_EN_2;
		en_2.individual.FIFO_OVERFLOW_EN_1 = write->FIFO_OVERFLOW_EN_1;
		en_2.individual.FIFO_OVERFLOW_EN_0 = write->FIFO_OVERFLOW_EN_0;
		en_3.individual.FIFO_WM_EN_4 = write->FIFO_WM_EN_4;
		en_3.individual.FIFO_WM_EN_3 = write->FIFO_WM_EN_3;
		en_3.individual.FIFO_WM_EN_2 = write->FIFO_WM_EN_2;
		en_3.individual.FIFO_WM_EN_1 = write->FIFO_WM_EN_1;
		en_3.individual.FIFO_WM_EN_0 = write->FIFO_WM_EN_0;

        retval = write_register(AGB0_REG_INT_ENABLE, (uint8_t*)&en_0, sizeof(ICM_20948_INT_ENABLE_t));
        if (retval != Status_Ok) {
            return retval;
        }
        retval = write_register(AGB0_REG_INT_ENABLE_1, (uint8_t*)&en_1, sizeof(ICM_20948_INT_ENABLE_1_t));
        if (retval != Status_Ok) {
            return retval;
        }
        retval = write_register(AGB0_REG_INT_ENABLE_2, (uint8_t*)&en_2, sizeof(ICM_20948_INT_ENABLE_2_t));
        if (retval != Status_Ok) {
            return retval;
        }
        retval = write_register(AGB0_REG_INT_ENABLE_3, (uint8_t*)&en_3, sizeof(ICM_20948_INT_ENABLE_3_t));
		if (retval != Status_Ok) {
            return retval;
        }
    }

    // if read is not null, read registers
    // if write is not null then this should read back the results of write into read
    if (read) {
        retval = read_register(AGB0_REG_INT_ENABLE, (uint8_t*)&en_0, sizeof(ICM_20948_INT_ENABLE_t));
        if (retval != Status_Ok) {
            return retval;
        }
		retval = read_register(AGB0_REG_INT_ENABLE_1, (uint8_t*)&en_1, sizeof(ICM_20948_INT_ENABLE_1_t));
        if (retval != Status_Ok) {
            return retval;
        }
		retval = read_register(AGB0_REG_INT_ENABLE_2, (uint8_t*)&en_2, sizeof(ICM_20948_INT_ENABLE_2_t));
        if (retval != Status_Ok) {
            return retval;
        }
		retval = read_register(AGB0_REG_INT_ENABLE_3, (uint8_t*)&en_3, sizeof(ICM_20948_INT_ENABLE_3_t));
		if (retval != Status_Ok) {
			return retval;
		}

        read->I2C_MST_INT_EN = en_0.I2C_MST_INT_EN;
		read->DMP_INT1_EN = en_0.DMP_INT1_EN;
		read->PLL_RDY_EN = en_0.PLL_READY_EN;
		read->WOM_INT_EN = en_0.WOM_INT_EN;
		read->REG_WOF_EN = en_0.REG_WOF_EN;
		read->RAW_DATA_0_RDY_EN = en_1.RAW_DATA_0_RDY_EN;
		read->FIFO_OVERFLOW_EN_4 = en_2.individual.FIFO_OVERFLOW_EN_4;
		read->FIFO_OVERFLOW_EN_3 = en_2.individual.FIFO_OVERFLOW_EN_3;
		read->FIFO_OVERFLOW_EN_2 = en_2.individual.FIFO_OVERFLOW_EN_2;
		read->FIFO_OVERFLOW_EN_1 = en_2.individual.FIFO_OVERFLOW_EN_1;
		read->FIFO_OVERFLOW_EN_0 = en_2.individual.FIFO_OVERFLOW_EN_0;
		read->FIFO_WM_EN_4 = en_3.individual.FIFO_WM_EN_4;
		read->FIFO_WM_EN_3 = en_3.individual.FIFO_WM_EN_3;
		read->FIFO_WM_EN_2 = en_3.individual.FIFO_WM_EN_2;
		read->FIFO_WM_EN_1 = en_3.individual.FIFO_WM_EN_1;
		read->FIFO_WM_EN_0 = en_3.individual.FIFO_WM_EN_0;
    }

    return retval;
}

Status_e IMU::int_enable_i2c(bool enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }

    en.I2C_MST_INT_EN = enable; // change the setting
    status = int_enable(&en, &en); // write phase w/ readback
    if (status != Status_Ok) {
        return status;
    }

    if (en.I2C_MST_INT_EN != enable) {
        status = Status_Err;
    }

    return status;
}

Status_e IMU::int_enable_DMP(bool enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }

    en.DMP_INT1_EN = enable; // change the setting
    status = int_enable(&en, &en); // write phase w/ readback
    if (status != Status_Ok) {
        return status;
    }

    if (en.DMP_INT1_EN != enable) {
        status = Status_Err;
    }

    return status;
}

Status_e IMU::int_enable_PLL(bool enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }
    
    en.PLL_RDY_EN = enable; // change the setting
    status = int_enable(&en, &en); // write phase w/ readback
    if (status != Status_Ok) {
        return status;
    }

    if (en.PLL_RDY_EN != enable) {
        status = Status_Err;
    }

    return status;
}

Status_e IMU::int_enable_WOM(bool enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }

    en.WOM_INT_EN = enable; // change the setting
    status = int_enable(&en, &en); // write phase w/ readback
    if (status != Status_Ok) {
        return status;
    }

    if (en.WOM_INT_EN != enable) {
        status = Status_Err;
    }

    return status;
}

Status_e IMU::int_enable_WOF(bool enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }

    en.REG_WOF_EN = enable; // change the setting
    status = int_enable(&en, &en); // write phase w/ readback
    if (status != Status_Ok) {
        return status;
    }

    if (en.REG_WOF_EN != enable) {
        status = Status_Err;
    }

    return status;
}

Status_e IMU::int_enable_raw_data_ready(bool enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }

    en.RAW_DATA_0_RDY_EN = enable; // change the setting
    status = int_enable(&en, &en); // write phase w/ readback
    if (status != Status_Ok) {
        return status;
    }

    if (en.RAW_DATA_0_RDY_EN != enable) {
        status = Status_Err;
    }

    return status;
}

Status_e IMU::int_enable_overflow_FIFO(uint8_t bm_enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }

    en.FIFO_OVERFLOW_EN_0 = ((bm_enable >> 0) & 0x01); // change the settings
    en.FIFO_OVERFLOW_EN_1 = ((bm_enable >> 1) & 0x01);
    en.FIFO_OVERFLOW_EN_2 = ((bm_enable >> 2) & 0x01);
    en.FIFO_OVERFLOW_EN_3 = ((bm_enable >> 3) & 0x01);
    en.FIFO_OVERFLOW_EN_4 = ((bm_enable >> 4) & 0x01);
    
    status = int_enable(&en, &en); // write phase w/ readback
    return status;
}

Status_e IMU::int_enable_watermark_FIFO(uint8_t bm_enable) {
    INT_enable_t en; // storage
    status = int_enable(NULL, &en); // read phase
    if (status != Status_Ok) {
        return status;
    }

    en.FIFO_WM_EN_0 = ((bm_enable >> 0) & 0x01); // change the settings
    en.FIFO_WM_EN_1 = ((bm_enable >> 1) & 0x01);
    en.FIFO_WM_EN_2 = ((bm_enable >> 2) & 0x01);
    en.FIFO_WM_EN_3 = ((bm_enable >> 3) & 0x01);
    en.FIFO_WM_EN_4 = ((bm_enable >> 4) & 0x01);
    
    status = int_enable(&en, &en); // write phase w/ readback
    return status;
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