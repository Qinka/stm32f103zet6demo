/**
 * beep driver
 */
#define _BEEP_C_
#include <firmware/stm32f10x.h>
#include <firmware/stm32f10x_gpio.h>
#include <firmware/stm32f10x_rcc.h>
#include <hardware/beep.h>

void beep_init(void) {
  // initialize clock
  RCC_APB2PeriphClockCmd(_BEEP_CLK_,ENABLE);
  GPIO_InitTypeDef BEEP_GPIO;
  BEEP_GPIO.GPIO_Pin   = _BEEP_PIN_;
  BEEP_GPIO.GPIO_Mode  = GPIO_Mode_Out_PP;
  BEEP_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(_BEEP_PORT_,&BEEP_GPIO);
  beep_on();
  for(int i = 0xFFFF; i>0 ;--i);
}

void beep_off(void) {
  GPIO_ResetBits(_BEEP_PORT_,GPIO_Pin_0);
}

void beep_on(void) {
  GPIO_SetBits(_BEEP_PORT_,GPIO_Pin_0);
}
