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
  uint32_t data; 
  return manufacturer_write(SBS_CMD.MANUFACTURER_INFO , data, 0);
}

BQ40Z80::unseal()
{
  //should be initialized with constructor
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

  return m_smbus.block_write(SBS_MANUFACTURER_BLOCK_ACCESS, buf, length +2);
}

BQ40Z80::getAllCellStatus()
{
  uint8_t DAstatus1[32 + 2] = {};
  uint8_t DAstatus3[18 + 2] = {};

  int status = manufacturer_read(DA_STATUS_1, DAstatus1, sizeof(DAstatus1)); 

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
   status = manufacturer_read(DA_STATUS_3, DAstatus3, sizeof(DAstatus3));
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

  int status = manufacturer_read(DA_STATUS_2, DAstatus2, sizeof(DAstatus2));

  float temp_sensor_1 = ((float)((DAstatus2[3] << 8) | DAstatus2[2]) / 1000); 
  
  return status;
}

BQ40Z80::getStartupInfo()
{


}

BQ40Z80::lifetimeDataFlush()
{
  return manufacturer_write(MA_LIFETIME_DATA_FLUSH, NULL, 0);
}


} //namespace BQ40Z80

