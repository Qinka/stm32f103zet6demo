/**
 * The functions for delay by using system clock
 */

// includes
#include "stm32f10x.h"
#include "delay.h"

// times for delay
static u8  Fac_us=0;//us
static u16 Fac_ms=0;//ms


/**
 * The initialization for delay
 *  SYSCLK system clock
 * SYSTICK => HCLK 1/8
 */
void delayInit(void)	{
  // peripheral clock HCLK (div by 8) => HCLK / 8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  // set it up to system's clock's eighth (for mu-second)
	Fac_us = SystemCoreClock / 8000000;
  // the system-tick for every micro-second
	Fac_ms = (u16)Fac_us * 1000;
}

/**
 * function for delay n us
 * @param N_us mu-seconds for delay
 */
void delay_us(unsigned long N_us) {
	u32 temp;
  // load time
	SysTick -> LOAD  = N_us * Fac_us;
  // clean up the counter
	SysTick -> VAL   = 0x00;
  // enable to count down
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;
	do { //empty loop for time up 
		temp = SysTick -> CTRL;
	}
	while(temp & 0x01 && !(temp & (0b1 << 16)));
  // disable timer
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  // clean up timer
	SysTick->VAL = 0X00;
}

/**
 * function for delay n ms
 * @param N_ms micro-seconds for delay N_ms <= 1864 (for 72MHz)
 *   the max for N_ms <= 0xffffff * 8 * 1000 / SYSCLK
 * 
 */
void delay_ms(unsigned int N_ms) {
	u32 temp;
  // load time
	SysTick -> LOAD  = (u32)N_ms * Fac_ms;
  // clean timer
	SysTick -> VAL   = 0x00;
  // enable the count down
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;
	do { //empty loop for time up 
		temp = SysTick -> CTRL;
	}	while(temp & 0x01 && !(temp & (0b1 << 16)));
  // disable the timer
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  // clean the timer
	SysTick -> VAL = 0X00;
}

/**
 * funciton for delay n s
 * @param N_s seconds for delay
 * N_s <= 255;
 */
void delay_s(unsigned int N_s) {
	for(u8 i = 0; i < N_s; i++) delay_ms(1000);
}
