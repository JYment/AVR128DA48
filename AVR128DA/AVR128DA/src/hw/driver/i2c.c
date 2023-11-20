/*
 * i2c.c
 *
 * Created: 2023-11-11 오후 2:08:34
 *  Author: jymen
 */ 

#include "i2c.h"
#include "cli.h"
#define BAUD(F_SCL, T_RISE)                        ((((((float)F_CPU / (float)F_SCL)) - 10 - ((float)F_CPU * T_RISE / 1000000))) / 2)


void cliI2c(cli_args_t *args);



bool i2cInit(void)
{
	PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
	PORTA.DIRSET = (1 << 2) | (1 << 3);
	cliAdd("i2c", cliI2c);
	
	return true;
}

bool i2cIsInit(void)
{
	
}

bool i2cBegin(uint8_t ch, uint32_t freq_khz)
{
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MBAUD = (uint8_t)BAUD(freq_khz, 0); /* set MBAUD register */
		TWI0.MCTRLA = 1 << TWI_ENABLE_bp        /* Enable TWI Master: enabled */
					| 0 << TWI_QCEN_bp			/* Quick Command Enable: disabled */
					| 0 << TWI_RIEN_bp			/* Read Interrupt Enable: disabled */
					| 0 << TWI_SMEN_bp			/* Smart Mode Enable: disabled */
					| TWI_TIMEOUT_DISABLED_gc	/* Bus Timeout Disabled */
					| 0 << TWI_WIEN_bp;			/* Write Interrupt Enable: disabled */
					
		TWI0.MCTRLB  |= TWI_FLUSH_bm;
		TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
		break;
	}
	
	return true;
}

bool i2cStart(uint8_t ch)
{
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MCTRLB  |= TWI_FLUSH_bm;
		TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
		break;
	}
	
	return true;
}
 
// void i2cReset(uint8_t ch);
bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
	
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
		TWI0.MADDR    = dev_addr << 1;
		while (!(TWI0.MSTATUS & TWI_RIF_bm) && !(TWI0.MSTATUS & TWI_WIF_bm));
		if(!(TWI0.MSTATUS & TWI_RXACK_bm))
			return true;
		break;
	}
	
	return false;
}

// bool i2cRecovery(uint8_t ch);
// bool i2cReadByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t timeout);
// bool i2cReadBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// bool i2cReadA16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// 
bool i2cWriteByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t data, uint32_t timeout)
{
	switch(ch)
	{
		case _DEF_I2C1:
		i2cStart(ch);
		i2cIsDeviceReady(ch, dev_addr);
		
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);	
		TWI0.MDATA = reg_addr;
		while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
		if(TWI0.MSTATUS & TWI_RXACK_bm)
			return false;
			
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
		TWI0.MDATA = data;
		while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
		if(TWI0.MSTATUS & TWI_RXACK_bm)
			return false;
		
		i2cEnd(ch);
		break;
	}
	
	return true;
}
bool i2cWriteBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
	switch(ch)
	{
		case _DEF_I2C1:
		
		i2cStart(ch);
		i2cIsDeviceReady(ch, dev_addr);
		
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
		TWI0.MDATA = reg_addr;
		while (!(TWI0.MSTATUS & TWI_WIF_bm));
		if(TWI0.MSTATUS & TWI_RXACK_bm)
		return false;
		
		while(length--)
		{
			TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
			TWI0.MDATA = *p_data;
			while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
			if(TWI0.MSTATUS & TWI_RXACK_bm)
				return false;	
				
			p_data++;
		}
		
		
		i2cEnd(ch);
		break;
	}
}
// bool i2cWriteA16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
// 
// bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);

bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
	switch(ch)
	{
		case _DEF_I2C1:
		i2cStart(ch);
		i2cIsDeviceReady(ch, dev_addr);
			
		while(length--)
		{
			TWI0.MDATA = *p_data;
			while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
			if(TWI0.MSTATUS & TWI_RXACK_bm)
				return false;
			*p_data++;
		}
		
		break;
	}
	
	return true;	
}

bool i2cEnd(uint8_t ch)
{
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		break;
	}
}



void cliI2c(cli_args_t *args)
{
	
	bool ret = false;

	if (args->argc == 1 && args->isStr(0, "scan") == true)
	{
		for(int i=0x00; i<=0x7F; i++)
		{
			if(i2cIsDeviceReady(_DEF_I2C1, i) == true)
			{
				cliPrintf_P(PSTR("[OK] Addr : 0x%X\n"), i);
			}
		}
		
		ret = true;
	}

	if (ret != true)
	{
		cliPrintf_P(PSTR("i2c scan\n"));
	}
}