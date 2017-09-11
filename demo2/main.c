#include "stm32f10x.h"
#include "config.h"

void loopDelay(void);

int _exit(void)
{
  return 0;
}

int main(void)
{
  // init GPIO
  // init clock                                     
  RCC -> APB2ENR |= (0b1 << _LED_1_GPIO_CLK_) | (0b1 << _LED_2_GPIO_CLK_);
  // clean gpio (PB0)
  _LED_1_GPIO_SET_ -> _LED_1_GPIO_CR_ &= ~ (0xF << _LED_1_GPIO_CRI_);
  _LED_1_GPIO_SET_ -> _LED_1_GPIO_CR_ |= 0b0011 << _LED_1_GPIO_CRI_;
  // clean gpio (PF7)
  _LED_2_GPIO_SET_ -> _LED_2_GPIO_CR_ &= ~ (0xF << _LED_2_GPIO_CRI_);
  _LED_2_GPIO_SET_ -> _LED_2_GPIO_CR_ |= 0b0011 << _LED_2_GPIO_CRI_;
  // init Timer 3(TIM3)
  RCC -> APB1ENR |= 0b1 << 1;
  TIM3 -> PSC = 7199; // 7.2K for 72MHz => 10KHz (1/10 ms)
  TIM3 -> ARR = 4999; // 5K for 500ms
  TIM3 -> DIER |= 0b1 << 0;
  TIM3 -> CR1 |= 0b1 << 0;
  // init for nvic (group 2, p (2,2))
  u32 tmp = SCB -> AIRCR;
  tmp &= 0xF8FF;
  tmp |= 0x05FA << 16;
  tmp |= 0x0200; // ((~ 2) & 0b111) << 8
  SCB -> AIRCR = tmp;
  NVIC -> ISER[TIM3_IRQn /32] |= (0b1 << TIM3_IRQn % 32);
  NVIC -> IP[TIM3_IRQn] |= 0b1010; // (pp << (4 - g) | ((rp & (0xf >> g))& 0xf
  while(1)
  {
    _LED_1_GPIO_SET_ -> ODR &= ~(0b1 << _LED_1_GPIO_PORT_);
    loopDelay();
    _LED_1_GPIO_SET_ -> ODR |= 0b1 << _LED_1_GPIO_PORT_;
    loopDelay();
  }
  return 0;
}

void TIM3_IRQHandler(void)
{
  if (TIM3 -> SR & 0b1)
  {
    _LED_2_GPIO_SET_ -> ODR ^= (0b1 << _LED_2_GPIO_PORT_);
  }
  TIM3 -> SR &= ~(0b1 << 0);
}


void loopDelay(void)
{
  for(u32 i = 0xFFFFFF; i != 0; --i);
}
  