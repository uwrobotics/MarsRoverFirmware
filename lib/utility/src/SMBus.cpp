/****************************************************************************
 *
 *   Copyright (c) 2018 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file SMBus.cpp
 * SMBus v2.0 protocol implementation.
 *
 * @author Jacob Dahl <dahl.jakejacob@gmail.com>
 * @author Bazooka Joe <BazookaJoe1900@gmail.com>
 *
 * TODO
 *  - Enable SMBus mode at the NuttX level. This may be tricky sharing the bus with i2c.
 *  	i.e STM32f4 see platforms/nuttx/Nuttx/nuttx/arch/arm/src/stm32/stm32f40xxx_i2c.c
 *  - Add remaining SMBus protocol messages as needed
 */

//Modified by Felix Wong for UW Robotics Firwmare Team
//TODO swap transfer for mbed i2c command

#include "SMBus.h"

SMBus::SMBus(PinName sda, PinName scl, uint16_t address)
{
	m_address = address;
}

SMBus::~SMBus(){}

int SMBus::read_word(const uint8_t cmd_code, uint16_t &data)
{
	uint8_t buf[6];
	// 2 data bytes + pec byte
	buf[0] = cmd_code;

	m_i2c.write(m_address, buf, 1);
	m_i2c.read(m_address, buf, 3);

	data = buf[3] | ((uint16_t)buf[4] << 8);
	// Check PEC.
	uint8_t addr = m_address << 1;
	buf[0] = addr | 0x00;
	buf[1] = cmd_code;
	buf[2] = addr | 0x01;

	uint8_t pec = get_pec(buf, sizeof(buf) - 1);

	if (pec != buf[sizeof(buf) - 1]) {
		result = -EINVAL;
	}
	return 0;
}

int SMBus::write_word(const uint8_t cmd_code, uint16_t data)
{
	// 2 data bytes + pec byte
	uint8_t buf[5];
	buf[0] = (m_address << 1) | 0x10;
	buf[1] = cmd_code;
	buf[2] = data & 0xff;
	buf[3] = (data >> 8) & 0xff;

	buf[4] = get_pec(buf, 4);

	//not sure if buf[0] should be used. may be able to just use address from class
 	return m_i2c.write(m_address, buf, 5);
}

int SMBus::block_read(const uint8_t cmd_code, void *data, const uint8_t length, const bool use_pec)
{
	uint8_t byte_count = 0;
	// addr(wr), cmd_code, addr(r), byte_count, data (MAX_BLOCK_LEN bytes max), pec
	uint8_t rx_data[MAX_BLOCK_LEN + 5];

	if (length > MAX_BLOCK_LEN) {
		return -EINVAL;
	}

	rx_data[0] = cmd_code;

	m_i2c.write(m_address, rx_data, 1)
	m_i2c.read(m_address, rx_data, length+2);

	rx_data[0] = (m_address << 1) | 0x00;
	rx_data[1] = cmd_code;
	rx_data[2] = (m_address << 1) | 0x01;
	byte_count = std::min(rx_data[3], MAX_BLOCK_LEN);

	// ensure data is not longer than given buffer
	memcpy(data, &rx_data[4], std::min(byte_count, length));

	if (use_pec) {
		uint8_t pec = get_pec(rx_data, byte_count + 4);

		if (pec != rx_data[byte_count + 4]) {
			result = -EIO;
		}
	}

	return 0;
}

int SMBus::block_write(const uint8_t cmd_code, const void *data, uint8_t byte_count, const bool use_pec)
{
	// cmd code[1], byte count[1], data[byte_count] (MAX_BLOCK_LEN max), pec[1] (optional)
	uint8_t buf[MAX_BLOCK_LEN + 2];

	if (byte_count > MAX_BLOCK_LEN) {
		return -EINVAL;
	}

	buf[0] = cmd_code;
	buf[1] = (uint8_t)byte_count;
	memcpy(&buf[2], data, byte_count);

	if (use_pec) {
		uint8_t pec = get_pec(buf, byte_count + 2);
		buf[byte_count + 2] = pec;
		byte_count++;
	}

	unsigned i = 0;
	int result = 0;

	// If block_write fails, try up to 10 times.

	while (i < 10 && ((result = m_i2c.write(m_address, buf, byte_count+2)) != 0)) {
		i++;
	}

	if (i == 10 || result) {
		result = -EINVAL;
	}

	return result;
}

uint8_t SMBus::get_pec(uint8_t *buf, const uint8_t len)
{
	// TODO: use "return crc8ccitt(buff, len);"

	// Initialise CRC to zero.
	uint8_t crc = 0;
	uint8_t shift_register = 0;
	bool invert_crc;

	// Calculate crc for each byte in the stream.
	for (uint8_t i = 0; i < len; i++) 
	{
		// Load next data byte into the shift register
		shift_register = buff[i];

		// Calculate crc for each bit in the current byte.
		for (uint8_t j = 0; j < 8; j++) 
		{
			invert_crc = (crc ^ shift_register) & 0x80;
			crc <<= 1;
			shift_register <<= 1;

			if (invert_crc) 
			{
				crc ^= SMBUS_PEC_POLYNOMIAL;
			}
		}
	}

	return crc;
}
