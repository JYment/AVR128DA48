/*
 * hw.c
 *
 * Created: 2023-11-11 오후 12:28:15
 *  Author: jymen
 */ 

#include "hw.h"


bool hwInit(void)
{
	bspInit();
	
	ledInit();
	uartInit();
	cliInit();
	
	return true;
}