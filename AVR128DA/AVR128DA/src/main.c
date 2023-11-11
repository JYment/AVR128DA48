/*
 * AVR128DA.c
 *
 * Created: 2023-11-11 오전 11:49:37
 * Author : jymen
 */ 

#include "main.h"



int main(void)
{
	hwInit();
	apInit();

	apMain();
	
	return 0;
}


