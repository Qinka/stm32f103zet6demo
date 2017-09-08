#include "stm32f10x.h"
#include "config.h"


int _exit(void)
{
  return 0;
}
void loopDelay(void);

int main(void)
{
  // init GPIO
  // init clock
  //                        PB
  RCC -> APB2ENR |= (1 << _LED_1_GPIO_CLK_);
  RCC -> APB2ENR |= 1 << 6; // PE
  RCC -> APB2ENR |= 1 << 7; // PF
  RCC -> APB2ENR |= 1 << 0; // AFIO
  // clean gpio (PB0)
  _LED_1_GPIO_SET_ -> _LED_1_GPIO_CR_ &= ~ (0xF << _LED_1_GPIO_CRI_);
  _LED_1_GPIO_SET_ -> _LED_1_GPIO_CR_ |= 0b0011 << _LED_1_GPIO_CRI_;
  // clean gpio (PF7)
  GPIOF -> CRL &= 0x0FFFFFFF;
  GPIOF -> CRL |= 0b0011 << 28;
  // clean key (PC13)
  GPIOC -> CRH &= 0xFF0FFFFF;
  GPIOC -> CRH |= 0b1000 << 20;
  AFIO  -> EXTICR[3] &= ~(0x0F << 4);
  AFIO  -> EXTICR[3] |= 2 << 4;
  EXTI  -> IMR  |= 0b1 << 13;
  EXTI  -> FTSR |= 0b1 << 13;
  // init group
  u32 tmp = SCB -> AIRCR;
  tmp &= 0x0000F8FF;
  tmp |= 0x05FA000;
  tmp |= 0b010 << 8;
  SCB -> AIRCR = tmp;
  // tmp = 2 << 2  =  0b1000
  // tmp |= (3 & (0b1111 >> 2) = 0b11 & 0b11 = 0b11) = 0b1011
  // tmp &= 0x0f = 0b1011
  NVIC -> ISER[EXTI15_10_IRQn / 32] |= 1 << (EXTI15_10_IRQn % 32);
  NVIC -> IP[EXTI15_10_IRQn] |= 0b1011 << 4;

  GPIOF -> ODR |= 1 << 7;
  while (1) {
    _LED_1_GPIO_SET_ -> ODR |=    0b1 << _LED_1_GPIO_PORT_;
    loopDelay();
    _LED_1_GPIO_SET_ -> ODR &= ~ (0b1 << _LED_1_GPIO_PORT_);
    loopDelay();
  }
  return 0;
}


void EXTI15_10_IRQHandler(void)
{
  for(u32 i = 0xFFFFF; i !=0; --i); 
  if(GPIOC -> IDR ^ (0b1 << 13)) { 
    GPIOF -> ODR ^= (1 << 7);  
  
  }
  EXTI -> PR = 0b1 << 13;
}



void loopDelay(void)
{
  for(u32 i = 0xFFFFFF; i != 0; --i);
}
  
