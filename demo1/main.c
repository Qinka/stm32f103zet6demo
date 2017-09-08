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
  RCC -> APB2ENR |= (1 << _LED_1_GPIO_CLK_) | (1 << _LED_2_GPIO_CLK_) | (1 << _KEY_1_GPIO_CLK_);
  RCC -> APB2ENR |= 1 << 0; // AFIO
  // clean gpio (PB0)
  _LED_1_GPIO_SET_ -> _LED_1_GPIO_CR_ &= ~ (0xF << _LED_1_GPIO_CRI_);
  _LED_1_GPIO_SET_ -> _LED_1_GPIO_CR_ |= 0b0011 << _LED_1_GPIO_CRI_;
  // clean gpio (PF7)
  _LED_2_GPIO_SET_ -> _LED_2_GPIO_CR_ &= ~ (0xF << _LED_2_GPIO_CRI_);
  _LED_2_GPIO_SET_ -> _LED_2_GPIO_CR_ |= 0b0011 << _LED_2_GPIO_CRI_;
  // clean key (PC13)  
  _KEY_1_GPIO_SET_ -> _KEY_1_GPIO_CR_ &= ~ (0xF << _KEY_1_GPIO_CRI_);
  _KEY_1_GPIO_SET_ -> _KEY_1_GPIO_CR_ |= 0b0011 << _KEY_1_GPIO_CRI_;
  AFIO -> EXTICR[_KEY_1_EXTI_IDX_] &= ~(0xF << _KEY_1_EXTI_MOV_);
  AFIO -> EXTICR[_KEY_1_EXTI_IDX_] |= _KEY_1_EXTI_SEL_ << _KEY_1_EXTI_MOV_;
  EXTI -> IMR  |= 0b1 << _KEY_1_GPIO_PORT_;
  EXTI -> FTSR |= 0b1 << _KEY_1_GPIO_PORT_;
  // init group (group 2)
  u32 tmp = SCB -> AIRCR;
  tmp &= 0x0000F8FF;
  tmp |= 0x05FA000;
  tmp |= 0b010 << 8;
  SCB -> AIRCR = tmp;
  // tmp = 2 << 2  =  0b1000
  // tmp |= (3 & (0b1111 >> 2) = 0b11 & 0b11 = 0b11) = 0b1011
  // tmp &= 0x0f = 0b1011
  NVIC -> ISER[_KEY_1_IRQn_ / 32] |= 1 << (_KEY_1_IRQn_ % 32);
  NVIC -> IP[_KEY_1_IRQn_] |= 0b1011 << 4;

  _LED_2_GPIO_SET_ -> ODR |= 0b1 << _LED_2_GPIO_PORT_;
  _LED_1_GPIO_SET_ -> ODR |= 0b1 << _LED_1_GPIO_PORT_;
  while (1) {
    loopDelay();
    _LED_1_GPIO_SET_ -> ODR ^= 0b1 << _LED_1_GPIO_PORT_;
  }
  return 0;
}


void EXTI15_10_IRQHandler(void)
{
  for(u32 i = 0xFFFFF; i !=0; --i); 
  if(_KEY_1_GPIO_SET_ -> IDR ^ (0b1 << _KEY_1_GPIO_PORT_)) { 
    _LED_2_GPIO_SET_ -> ODR ^= (1 << _KEY_1_GPIO_PORT_);  
  
  }
  EXTI -> PR = 0b1 << _KEY_1_GPIO_PORT_;
}



void loopDelay(void)
{
  for(u32 i = 0xFFFFFF; i != 0; --i);
}
  
