/*
 * def.h
 *
 * Created: 2023-11-11 오후 12:28:07
 *  Author: jymen
 */ 


#ifndef DEF_H_
#define DEF_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>


#define F_CPU		24000000UL


#define _DEF_LED1			0
#define _DEF_LED2			1

#define _DEF_UART1			0

#define _DEF_I2C1			0

#define _DEF_LOW			0
#define _DEF_HIGH			1


#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


#ifndef map
#define map(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif


#endif /* DEF_H_ */