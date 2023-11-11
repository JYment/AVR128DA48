/*
 * i2c.c
 *
 * Created: 2023-11-11 오후 2:08:34
 *  Author: jymen
 */ 

#include "i2c.h"

#define TWI0_BAUD(F_SCL, T_RISE)                        ((((((float)CLK_PER / (float)F_SCL)) - 10 - ((float)CLK_PER * T_RISE))) / 2)


bool i2cInit(void)
{
	
}

bool i2cIsInit(void)
{
	
}

bool i2cBegin(uint8_t ch, uint32_t freq_khz)
{
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MBAUD = (uint8_t)TWI0_BAUD(freq_khz, 0);
		break;
		
	}
}

bool i2cIsBegin(uint8_t ch)
{
	
}
// 
// void i2cReset(uint8_t ch);
// bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr);
// bool i2cRecovery(uint8_t ch);
// bool i2cReadByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t timeout);
// bool i2cReadBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// bool i2cReadA16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// 
// bool i2cWriteByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t data, uint32_t timeout);
// bool i2cWriteBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// bool i2cWriteA16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// 
// bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// 
// 
// void     i2cSetTimeout(uint8_t ch, uint32_t timeout);
// uint32_t i2cGetTimeout(uint8_t ch);
// 
// void     i2cClearErrCount(uint8_t ch);
// uint32_t i2cGetErrCount(uint8_t ch);