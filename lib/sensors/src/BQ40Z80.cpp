#pragma once
#include "BQ40Z80.h"


namespace BQ40Z80 {

BQ40Z80::BQ40Z80(PinName SDA_PIN, SCL_PIN, uint16_t addr)
{
  m_keys[0] = 0x0414;
  m_keys[1] = 0x3672;
  m_address = addr;
}

BQ40Z80::seal()
{
  // write nothing
  uint32_t data; 
  return manufacturer_write(SBS_CMD.MANUFACTURER_INFO , data, 0);
}

BQ40Z80::unseal()
{
  //  uint16_t keys[2] = {0x0414, 0x3672};
  int status = m_smbus.write_word(SBS_MANUFACTURER_ACCESS, m_keys[0]);

  status |= m_smbus.write_word(SBS_MANUFACTURER_ACCESS, m_keys[1]); 
	
  return status;
}

BQ40Z80::manufacturer_read(const uint16_t sbs_cmd, uint32_t &data, const uint8_t length)
{

//write 0x44 ox 0x00 then the data sent is teh command
//block or word

  uint8_t command[2] = {};
  command[0] = ((uint8_t *)&sbs_cmd)[0];
  command[0] = ((uint8_t *)&sbs_cmd)[1];

  int status = m_smbus.block_write(SBS_MANUFACTURER_BLOCK_ACCESS, command, 2); 

  status = m_smbus.block_read(SBS_MANUFACTURER_BLOCK_ACCESS, data, length);

  //parse out address bytes
  //if using ptr can use mmove
  return status;
}

BQ40Z80::manufacturer_write(const uint16_t sbs_cmd, uint32_t &data, const uint8_t length)
{
//write 0x44 0x00 then the data sent is the data written
//block or word
 
  uint8_t buf[MAC_DATA_BUF_SIZE +2] = {};
  buf[0] = sbs_cmd & 0xff;
  buf[1] = (sbs_cmd >> 8) & 0xff;
  memcpy(&buf[2], data, length);

  return m_smbus.block_write(SBS_MANUFACTURER_BLOCK_ACCESS, buf, length +2);
}

BQ40Z80::getAllCellStatus()
{
  uint8_t DAstatus1[32 + 2] = {};
  uint8_t DAstatus3[18 + 2] = {};

  int status = manufacturer_read(SBS_MA_CMD.DA_STATUS_1, DAstatus1, sizeof(DAstatus1)); 

  m_cell_voltages[0] = ((float)((DAstatus1[1] << 8) | DAstatus1[0]) / 1000);
  m_cell_voltages[1] = ((float)((DAstatus1[3] << 8) | DAstatus1[2]) / 1000);
  m_cell_voltages[2] = ((float)((DAstatus1[5] << 8) | DAstatus1[4]) / 1000);
  m_cell_voltages[3] = ((float)((DAstatus1[7] << 8) | DAstatus1[6]) / 1000);

  m_cell_current[0] = ((float)((DAstatus1[13] << 8) | DAstatus1[12]) / 1000);
  m_cell_current[1] = ((float)((DAstatus1[15] << 8) | DAstatus1[14]) / 1000);
  m_cell_current[2] = ((float)((DAstatus1[17] << 8) | DAstatus1[16]) / 1000);
  m_cell_current[3] = ((float)((DAstatus1[19] << 8) | DAstatus1[18]) / 1000);

  m_cell_powers[0] = ((float)((DAstatus1[21] << 8) | DAstatus1[20]) / 1000);
  m_cell_powers[1] = ((float)((DAstatus1[23] << 8) | DAstatus1[22]) / 1000);
  m_cell_powers[2] = ((float)((DAstatus1[25] << 8) | DAstatus1[24]) / 1000);
  m_cell_powers[3] = ((float)((DAstatus1[27] << 8) | DAstatus1[26]) / 1000);

  m_total_power = ((float)((DAstatus1[29] << 8 | DAstatus1[28]) / 1000); 
  m_avg_power = ((float)((DAstatus1[31] << 8 | DAstatus1[30]) / 1000); 

  //can add cell current calculations and cell power calculations
  status = manufacturer_read(SBS_MA_CMD.DA_STATUS_3, DAstatus3, sizeof(DAstatus3));
  m_cell_voltages[4] = ((float)((DAstatus3[1] << 8) | DAstatus1[0]) / 1000);
  m_cell_voltages[5] = ((float)((DAstatus3[7] << 8) | DAstatus1[6]) / 1000);
  m_cell_current[4] = ((float)((DAstatus1[3] << 8) | DAstatus1[2]) / 1000);
  m_cell_current[5] = ((float)((DAstatus1[9] << 8) | DAstatus1[8]) / 1000);
  m_cell_powers[4] = ((float)((DAstatus1[5] << 8) | DAstatus1[4]) / 1000);
  m_cell_powers[5] = ((float)((DAstatus1[11] << 8) | DAstatus1[10]) / 1000);
 
  return status;
}

BQ40Z80::getTemperatures()
{
  uint8_t DAstatus2[14 + 2] = {};
  int status = manufacturer_read(SBS_MA_CMD.DA_STATUS_2, DAstatus2, sizeof(DAstatus2));
  m_temp_1 = ((float)((DAstatus2[3] << 8) | DAstatus2[2]) / 1000); 
  
  return status;
}

BQ40Z80::getStartupInfo()
{
  int status;

  status = m_smbus.read_word(SBS_MA_CMD.FIRMWARE_VERSION, m_firmware_version);

  status = m_smbus.read_word(SBS_MA_CMD.STATE_OF_HEALTH, m_state_of_health);

  status = m_smbus.read_word(SBS_MA_CMD.DEVICE_TYPE, m_device_type);

  status = m_smbus.read_word(SBS_CMD.CYCLE_COUNT, m_cycle_count);

  status = m_smbus.read_word(SBS_CMD.REMAINING_CAPACITY, m_remaining_capacity);

  status = m_smbus.read_word(SBS_CMD.FULL_CHARGE_CAPACITY, m_full_charge_capacity);

  return status;
}

BQ40Z80::lifetimeDataFlush()
{
  return manufacturer_write(SBS_MA_CMD.LIFETIME_DATA_FLUSH, NULL, 0);
}

BQ40Z80::readLifeTimeData()
{
  uint8_t data_blk[32+2] = {};

  int status = manufacturer_read(SBS_MA_CMD.LIFETIME_BLK_1, data_blk, sizeof(data_blk));

  //parse lifetime voltage data
  float cell_1_max_V =(float)(data_blk[1] << 8 | data_blk[0])/ 1000f;
  

  //parse other data blk 2
  status = manufacturer_read(MA_LIFETIME_BLK_2, data_blk, sizeof(data_blk));

  float max_charge_current = (float)(data_blk[1] << 8 | data_blk[0])/1000f;
  float max_discharge_current = (float)(data_blk[3] << 8 | data_blk[2])/1000f;
  float max_temp_cell = (float)(data_blk[8])/1000f;

  float min_temp_cell = (float)(data_blk[9])/1000f;

  int num_shutdowns = data_blk[14];

  int num_full_resets = data_blk[16];

  //bytes 18-24 contain the cell balancing time for each cel

  //block 3 contains time spent in the different modes

  return status;
}


//==============================
//some get functions for basic battery data
//==============================

BQ40Z80::getCurrent(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.CURRENT, data); 
}

BQ40Z80::getVoltage(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.VOLTAGE, data);
}

BQ40Z80::getAvgCurrent(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.AVERAGE_CURRENT, data);
}

BQ40Z80::getTimeToEmpty(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.RUN_TIME_TO_EMPTY , data);
}

BQ40Z80::getAvgTimeToEmpty(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.AVERAGE_TIME_TO_EMPTY , data);
}

BQ40Z80::getRemainingCapacity(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.REMAINING_CAPACITY , data);
}

BQ40Z80::getMaxError(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.MAX_ERROR , data);
}

BQ40Z80::getTemp(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.TEMPERATURE , data);
}

BQ40Z80::getRelativeSOC(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.RELATIVE_STATE_OF_CHANGE , data);
}

BQ40Z80::getAbsSOC(uint32_t & data)
{
  return m_smbus.read_word(SBS_CMD.ABS_STATE_OF_CHANGE , data);
}

} //namespace BQ40Z80

