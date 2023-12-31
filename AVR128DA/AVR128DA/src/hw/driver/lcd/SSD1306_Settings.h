﻿#ifndef SSD1306_SETTINGS_H_INCLUDED
#define SSD1306_SETTINGS_H_INCLUDED
/*
||
||  Filename:	 		SSD1306_Settings.h
||  Title: 			  SSD1306 Driver Settings
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:
||	Settings for the SSD1306 driver. Pick a size and the
||	desirable pins.
||
||	Size			Code
||--------------------------
||	128x64	-	GLCD_128_64
||	128x32	-	GLCD_128_32
||	 96x16	-	GLCD_96_16
||
*/

//----- Configuration -------------//
//Comment out and edit if there is RST pin
//#define GLCD_RST				A, 0

#define GLCD_Size         GLCD_128_32

//Specify whether error checking while transceiving data via I2C should be done. 
//#define GLCD_Error_Checking	

//Match auxiliary functions to your defined I2C functions
//Setup TWI peripheral at 400KHz
#define __I2C_Setup()			TWI_Setup()
//Transmit START signal
#define __I2C_Start()			i2cStart(_DEF_I2C1)
//Transmit STOP signal
#define __I2C_Stop()			i2cEnd(_DEF_I2C1)
//Transmit DATA
#define __I2C_Transmit(Data)	i2cSendData(_DEF_I2C1, Data)
//Receive DATA + Return ACK
#define __I2C_ReceiveACK()		TWI_ReceiveACK()
//Receive DATA + Return NACK
#define __I2C_ReceiveNACK()		TWI_ReceiveNACK()
//Return TWI Status Register (If you do not need error checking, leave it as is or delete it)
#define __I2C_Status()			TWI0.MSTATUS
//---------------------------------//
#endif
