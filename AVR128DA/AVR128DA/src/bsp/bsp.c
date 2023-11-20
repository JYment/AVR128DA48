/*
 * bsp.c
 *
 * Created: 2023-11-11 오후 12:27:26
 *  Author: jymen
 */ 

#include "bsp.h"



static volatile uint32_t ms_cnt = 0;


ISR(TCB0_INT_vect)
{
	ms_cnt++;
	TCB0.INTFLAGS = (1 << TCB_CAPT_bp);
}



bool bspInit(void)
{
	CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = (1 << CLKCTRL_ENABLE_bp);
		
	RTC.CLKSEL = (0x02 << RTC_CLKSEL_gp);
		
	CCP = CCP_IOREG_gc;
	CLKCTRL.OSCHFCTRLA = (0x09 << CLKCTRL_FREQSEL_gp)     // OSCHF = 24Mhz
					   | (0x01 << CLKCTRL_AUTOTUNE_bp);   // AUTOTUNE Enable
		
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = (0x00 << CLKCTRL_CLKSEL_gp);
		
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = (0x00 << CLKCTRL_PEN_bp);
		
		
	TCB0.CTRLA = (0x00 << TCB_CLKSEL_gp);
	TCB0.CTRLB = (0x00 << TCB_CNTMODE_gp);
	TCB0.INTCTRL = (1 << TCB_CAPT_bp);
	TCB0.CCMP = (1000 * (F_CPU / 1000000)) - 1;
	TCB0.CTRLA |= (1 << TCB_ENABLE_bp);
		
	PORTC.DIRSET = (1 << 6);
		
	sei();


	return true;
}


void delay(uint32_t time_ms)
{
	uint32_t preTime = millis();
	
	while(millis() - preTime < time_ms)
	{
		
	}
}

uint32_t millis(void)
{
	uint32_t ret;
	uint8_t old_sreg = SREG;
	
	cli();
	ret = ms_cnt;
	SREG = old_sreg;

	return ret;
}

void logPrintf(const char *fmt, ...);