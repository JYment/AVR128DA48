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
	PORTA.DIRSET = (1 << 2) | (1 << 3);
	PORTA.OUTCLR = (1 << 2) | (1 << 3);
	PORTMUX.TWIROUTEA = 0x00;	

	cliAdd("i2c", cliI2c);
	
	return true;
}


bool i2cBegin(uint8_t ch, uint32_t freq_khz)
{
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MBAUD  = (uint8_t)BAUD(freq_khz, 0); /* set MBAUD register */
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
	uint32_t pre_time = 0;
	uint8_t  timeout  = 10;
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MCTRLB   = TWI_FLUSH_bm;
		TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;
		
		pre_time = millis();
		while(1)
		{
			if((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) != TWI_BUSSTATE_BUSY_gc)
			{
				break;
			}
			if(millis() - pre_time >= timeout)
			{
				return false;
			}
		}	
		break;
	}
	
	return true;
}
 
bool i2cSendAddr(uint8_t ch, uint8_t dev_addr)
{
	bool     ret      = false;
	uint32_t pre_time = 0;
	uint8_t  timeout  = 10;
	
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
		TWI0.MADDR = dev_addr;
		
		pre_time = millis();
		while (!(TWI0.MSTATUS & TWI_RIF_bm) && !(TWI0.MSTATUS & TWI_WIF_bm))
		{
			if(millis() - pre_time >= timeout)
			{
				return false;
			}
		}
		
		if(!(TWI0.MSTATUS & TWI_RXACK_bm))
		{
			ret = true;
		}
		
		break;
	}
	
	return ret;
}

bool i2cSendData(uint8_t ch, uint8_t data)
{
	bool     ret      = false;
	uint32_t pre_time = 0;
	uint8_t  timeout  = 10;
	
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
		TWI0.MDATA = data;
		
		pre_time = millis();
		while (!(TWI0.MSTATUS & TWI_WIF_bm))
		{
			if(millis() - pre_time >= timeout)
			{
				return false;
			}
		}
		
		if(!(TWI0.MSTATUS & TWI_RXACK_bm))
		{
			ret = true;
		}
		
		break;
	}
	
	return ret;
}
 
bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
	
	switch(ch)
	{
		case _DEF_I2C1:
		if(i2cStart(ch) != true)
		{
			return false;
		}
		
		if(i2cSendAddr(ch, (dev_addr << 1)) != true)
		{
			return false;
		}
		
		if(i2cEnd(ch) != true)
		{
			return false;
		}
		break;
	}
	
	return true;
}


bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
	bool ret = false;
	
	switch(ch)
	{
		case _DEF_I2C1:
		if(i2cStart(ch) != true)
		{
			return false;
		}
		
		if(i2cSendAddr(ch, (dev_addr << 1)) != true)
		{
			return false;
		}
		
		while(length--)
		{
			if(i2cSendData(ch, *p_data) != true)
			{
				i2cEnd(ch);
				return false;
			}
			p_data++;
		}
		
		i2cEnd(ch);
		break;
	}
	
	return true;	
}

bool i2cEnd(uint8_t ch)
{
	uint32_t pre_time = 0;
	uint8_t  timeout  = 10;
	
	switch(ch)
	{
		case _DEF_I2C1:
		TWI0.MCTRLB &= 0xFC;
		TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
		
		pre_time = millis();
		while(1)
		{
			if((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) != TWI_BUSSTATE_BUSY_gc)
			{
				break;
			}
			if(millis() - pre_time >= timeout)
			{
				return false;
			}
		}
		break;
	}
	
	return true;
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


 
