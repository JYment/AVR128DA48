﻿/*
 * ap.c
 *
 * Created: 2023-11-11 오후 12:27:09
 *  Author: jymen
 */ 

#include "ap.h"


void cliTest(cli_args_t *args);


void apInit(void)
{
	cliOpen(_DEF_UART1, 115200);
	
	cliAdd("test", cliTest);
}

void apMain(void)
{
	uint32_t preTime;
	
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

void cliTest(cli_args_t *args)
{
	bool ret = false;

	if (args->argc == 1 && args->isStr(0, "show") == true)
	{
		cliPrintf_P(PSTR("this is show command \n"));
		ret = true;
	}

	if (args->argc == 2 && args->isStr(0, "show") == true)
	{
		uint8_t number;

		number = args->getData(1);

		cliPrintf_P(PSTR("this is show number %d \n"), number);
		ret = true;
	}

	if (ret != true)
	{
		cliPrintf_P(PSTR("test show\n"));
		cliPrintf_P(PSTR("test show 0~10\n"));
	}
}