#include "stm32f10x.h"
#include "config.h"
#include <string.h>

void loopDelay(void);

int _exit(void)
{
  return 0;
}

int main(void)
{
  // init gpio for led
  RCC -> APB2ENR |= 0b1 << _LED_GPIO_CLK_;
  _LED_GPIO_SET_ -> _LED_GPIO_CR_ &= ~(0xF << _LED_GPIO_CR_MOV_);
  _LED_GPIO_SET_ -> _LED_GPIO_CR_ |=   0xF << _LED_GPIO_CR_MOV_;
  // enable AF for LED port
  RCC -> APB2ENR |= 0b1 << 0;
  AFIO -> MAPR &= ~(_PWM_REMAP_ << _PWM_REMAP_MOV_);
  AFIO -> MAPR |= _PWM_REMAP_ << _PWM_REMAP_MOV_;
  // enable timer
  RCC -> APB1ENR |= 0b1 << _PWM_CLK_;
  _PWM_TIMx_ -> ARR = 899; // ARR -> 899 + 1 = 900 => 72Mhz / 900 = 80KHz
  _PWM_TIMx_ -> PSC = 0;
  _PWM_TIMx_ -> _PWM_CCMR_ |= 0b111 << (_PWM_CCMR_MOV_ + 1);
  _PWM_TIMx_ -> _PWM_CCMR_ |=   0b1 << _PWM_CCMR_MOV_;
  _PWM_TIMx_ -> CCER |= 0b1 << ((_PWM_CHx_ - 1) * 4);
  _PWM_TIMx_ -> CR1  = 0x0080;
  _PWM_TIMx_ -> CR1 |= 0b1;
  int16_t level=0;
  int8_t direction = 1;
  while(1){
    loopDelay();
    _PWM_TIMx_ -> _PWM_CCR_ = level;
    if (level >= _PWM_LVL_MAX_)
      direction = -1;
    if (level <= 0)
      direction = 1;
    level += direction;
  }
  return 0;
}


void loopDelay(void)
{
  for (u32 i = 0xFFFF; i > 0; --i);
}

  
