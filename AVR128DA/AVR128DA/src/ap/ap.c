/*
 * ap.c
 *
 * Created: 2023-11-11 오후 12:27:09
 *  Author: jymen
 */ 

#include "ap.h"

void ssd1306_command(uint8_t c)
{
	uint8_t control = 0x00;
	i2cStart(_DEF_I2C1);
	i2cSendAddr(_DEF_I2C1, 0x3C << 1);
	i2cSendData(_DEF_I2C1, control);
	i2cSendData(_DEF_I2C1, c);
	i2cEnd(_DEF_I2C1);
}

void apInit(void)
{
	cliOpen(_DEF_UART1, 115200);
	i2cBegin(_DEF_I2C1, 100000);
}

void apMain(void)
{
	uint32_t preTime;
	
	ssd1306_command(0xAE);
	ssd1306_command(0xD5);
	ssd1306_command(0x80);
	ssd1306_command(0xA8);
	ssd1306_command(32 - 1);
	ssd1306_command(0xD3);
	ssd1306_command(0x0);
	ssd1306_command(0x40 | 0x0);	
	ssd1306_command(0x8D);	
	ssd1306_command(0x14);		//external vcc
	ssd1306_command(0x20);	
	ssd1306_command(0x0);	
	ssd1306_command(0xA0 | 0x1);	
	ssd1306_command(0xC8);	
	ssd1306_command(0xDA);	
	ssd1306_command(0x02);	
	ssd1306_command(0x81);	
	ssd1306_command(0x8F);	
	ssd1306_command(0xD9);	
	ssd1306_command(0xF1);		//external vcc
	ssd1306_command(0xDB);	
	ssd1306_command(0x40);	
	ssd1306_command(0xA4);	
	ssd1306_command(0xA6);
	ssd1306_command(0x2E);	
	ssd1306_command(0xAF);
	
	
	ssd1306_command(0x8D);
	ssd1306_command(0x14);		//external vcc
	ssd1306_command(0xA5);
	ssd1306_command(0xAF);
	
	preTime = millis();
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
