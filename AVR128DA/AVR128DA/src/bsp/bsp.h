﻿/*
 * bsp.h
 *
 * Created: 2023-11-11 오후 12:27:33
 *  Author: jymen
 */ 


#ifndef BSP_H_
#define BSP_H_



#include "def.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/power.h>


bool bspInit(void);
bool bspDeInit(void);


void delay(uint32_t time_ms);
uint32_t millis(void);

void logPrintf(const char *fmt, ...);



#endif /* BSP_H_ */