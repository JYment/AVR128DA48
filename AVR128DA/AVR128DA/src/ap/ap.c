/*
 * ap.c
 *
 * Created: 2023-11-11 오후 12:27:09
 *  Author: jymen
 */ 

#include "ap.h"



void apInit(void)
{
	cliOpen(_DEF_UART1, 115200);
	i2cBegin(_DEF_I2C1, 100000);
}

void apMain(void)
{
	uint32_t preTime;
	
	preTime = millis();

	uint8_t cmd_buf[2];
	uint8_t cmd;
	
	cmd_buf[0] = 0x8D;		// charge pump
	cmd_buf[1] = 0x10 | (1 ? 4 : 0);
	i2cWriteBytes(_DEF_I2C1, 0x3C, 0, cmd_buf, 2, 100);
	
	cmd = 0xA5;		// entire on
	i2cWriteData(_DEF_I2C1, 0x3C, &cmd, 1, 100);
	
	cmd = 0xAF;		// display on
 	i2cWriteData(_DEF_I2C1, 0x3C, &cmd, 1, 100);
	
	while (1)
	{
		if(millis() - preTime >= 100)
		{
			preTime = millis();
			ledToggle(_DEF_LED1);
		}
		
		cliMain();
	}
}
