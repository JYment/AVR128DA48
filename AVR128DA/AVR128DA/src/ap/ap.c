/*
 * ap.c
 *
 * Created: 2023-11-11 오후 12:27:09
 *  Author: jymen
 */ 

#include "ap.h"
#include <util/delay.h>

#define PAGE_NUM		4
#define COL_NUM			128
#define PATTERN_SZ		128
#define SSD1306_CMD_ADDR_MODE			0x20
#define SSD1306_CMD_COL_ADDR			0x21
#define SSD1306_CMD_ROW_ADDR			0x22
#define SSD1306_CMD_COL_LO_ADDR         0x00
#define SSD1306_CMD_COL_HI_ADDR         0x10
#define SSD1306_CMD_PAGE_START_ADDR     0xB0

enum
{
	HORIZONTAL,
	VERTICAL,
	PAGE
};

static uint8_t gddram[PAGE_NUM][COL_NUM];

void ssd1306_command(uint8_t *data, uint16_t length)
{
	uint8_t control = 0x00;
	i2cStart(_DEF_I2C1);
	i2cSendAddr(_DEF_I2C1, 0x3C << 1);
	i2cSendData(_DEF_I2C1, control);
	for(int i=0; i<length; i++)
	{
		i2cSendData(_DEF_I2C1, *data++);
	}
	i2cEnd(_DEF_I2C1);
}

void ssd1306_data(uint8_t *data, uint16_t length)
{
	uint8_t control = 0x40;
	
	i2cStart(_DEF_I2C1);
	i2cSendAddr(_DEF_I2C1, 0x3C << 1);
	i2cSendData(_DEF_I2C1, control);
	
	for(int i=0; i<length; i++)
	{
		i2cSendData(_DEF_I2C1, *data++);
	}
	i2cEnd(_DEF_I2C1);
}

void ssd1306_set_addr_mode(uint8_t mode)
{
	uint8_t cmd_buf[2];
	
	cmd_buf[0] = SSD1306_CMD_ADDR_MODE;
	cmd_buf[1] = mode;
	
	ssd1306_command(cmd_buf, 2);
}

void ssd1306_set_page_start(uint8_t page)
{
	uint8_t cmd = SSD1306_CMD_PAGE_START_ADDR | page;
	
	ssd1306_command(&cmd, 1);
}


void ssd1306_set_page_addr(uint8_t start, uint8_t end)
{
	uint8_t cmd_buf[3];
	
	cmd_buf[0] = SSD1306_CMD_ROW_ADDR;
	cmd_buf[1] = start;
	cmd_buf[2] = end;
	
	ssd1306_command(cmd_buf, 3);
}

void ssd1306_set_col_addr(uint8_t start, uint8_t end)
{
	uint8_t cmd_buf[3];
	
	cmd_buf[0] = SSD1306_CMD_COL_ADDR;
	cmd_buf[1] = start;
	cmd_buf[2] = end;
	
	ssd1306_command(cmd_buf, 3);
}

void ssd1306_set_column_addr(uint8_t column)
{
	uint8_t cmd;
	
	cmd = SSD1306_CMD_COL_LO_ADDR | (column & 0xF);
	ssd1306_command(&cmd, 1);
	
	cmd = SSD1306_CMD_COL_HI_ADDR | ((column >> 4) & 0xF);
	ssd1306_command(&cmd, 1);
}

void clear_screen(void)
{
	uint8_t page;
	uint8_t col;
	
	for(page=0; page<PAGE_NUM; page++)
		for(col=0; col<COL_NUM; col++)
			gddram[page][col] = 0;

			
	for(page=0; page<PAGE_NUM; page++)
	{
		ssd1306_set_page_start(page);
		ssd1306_data(&gddram[0][0], COL_NUM);
	}
}

void draw_dot(uint8_t val, uint8_t row, uint8_t col)
{
	uint8_t page = row >> 3;
	uint8_t *dot = &gddram[page][col];
	
	if(val)
	{
		*dot |= 1 << (row & 7);
	}
	else
	{
		*dot &= ~(1 << (row & 7));
	}
	
	ssd1306_set_page_start(page);
	ssd1306_set_column_addr(col);
	ssd1306_data(dot, 1);
}

void display_test_pattern()
{
	draw_dot(1, rand()&0x1F, rand()&0x7F);
}



void apInit(void)
{
	cliOpen(_DEF_UART1, 115200);
	i2cBegin(_DEF_I2C1, 100000);
}

void apMain(void)
{
	int cnt = 0;
	uint32_t preTime;
	
	GLCD_Setup();
	ssd1306_set_addr_mode(PAGE);
	clear_screen();
	
	preTime = millis();
	while (1)
	{
		
		if(cnt++ == 1000)
		{
			clear_screen();
			_delay_ms(500);
			cnt = 0;
		}
		display_test_pattern();
		
		if(millis() - preTime >= 10)
		{
			preTime = millis();
			ledToggle(_DEF_LED1);
			
		}
		
		cliMain();
	}
}
