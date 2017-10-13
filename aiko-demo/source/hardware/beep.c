/**
 * beep driver
 */
#include <firmware/stm32f10x.h>
#include <firmware/stm32f10x_gpio.h>
#include <firmware/stm32f10x_rcc.h>
#include <hardware/beep.h>

 void beep_config(void) {
   // initialize clock
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
   GPIO_InitTypeDef BEEP_GPIO; // PC0
   BEEP_GPIO.GPIO_Pin   = GPIO_Pin_0;
   BEEP_GPIO.GPIO_Mode  = GPIO_Mode_Out_PP;
   BEEP_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC,& BEEP_GPIO);
   beep_on();
   for(int i = 0xFFFF; i>0 ;--i);
 }
 void beep_off(void) {
  GPIO_ResetBits(GPIOC,GPIO_Pin_0);
 }
 void beep_on(void) {
  GPIO_SetBits(GPIOC,GPIO_Pin_0);
 }
