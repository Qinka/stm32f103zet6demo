#include "stm32f10x.h"

int _exit(void)
{
  return 0;
}
void sendCharCom(u8);

u8 t_counter=0;
int main(void)
{
  // serial for baud rate 115200
  // configure GPIOA's port(used by usart, and PA0)
  RCC -> APB2ENR |= 0b1 << 2;
  RCC -> APB2ENR |= 0b1 << 3;
  GPIOB -> CRL &= ~(0xF << 5);
  GPIOB -> CRL |= 0b11 << 5;
  GPIOB -> ODR |= 0b1 << 5;
  // enable USART1's clock
  RCC -> APB2ENR |= 0b1 << 14;
  // configure TIM5 clock
  RCC -> APB1ENR |= 0b1 << 3;
  // setting GPIOA IO
  GPIOA -> CRH &= 0XFFFFF00F;
  GPIOA -> CRH |= 0X000008B0;
  // reset USART1
  RCC -> APB2RSTR |=  0b1 << 14;
  RCC -> APB2RSTR &= ~(0b1 << 14);
  // config usart1
  USART1 -> BRR = 0x0271; // for mantissa 0x27; faction 0x01;
  USART1 -> CR1 |= 0x200C; // 1 bit for stop, without parity
  // setting PA0
  GPIOA -> CRL &= ~(0xF << 0);
  GPIOA -> CRL |= 0b1000 << 0;
  GPIOA -> ODR |= (0 << 0); // pull down
  // setting TIM5
  TIM5 -> ARR = 0xFFFF;
  TIM5 -> ARR = 72 - 1;
  TIM5 -> CCMR1 |= 0b1 << 0;
  TIM5 -> CCMR1 |= (0 << 4);
  TIM5 -> CCMR1 |= (0 << 10);
  TIM5 -> CCER |= (0 << 1);
  TIM5 -> CCER |= 0b1 << 0;
  TIM5 -> DIER |= 0b1 << 1;
  TIM5 -> DIER |= 0b1 << 0;
  TIM5 -> CR1 |= 0b1 << 0;
  // init interrupt for TIM5
  // nvic (group 2, p (2,2))
  u32 tmp = SCB -> AIRCR;
  tmp &= 0xF8FF;
  tmp |= 0x05FA << 16;
  tmp |= 0x0200; // ((~ 2) & 0b111) << 8
  SCB -> AIRCR = tmp;
  NVIC -> ISER[TIM5_IRQn /32] |= (0b1 << TIM5_IRQn % 32);
  NVIC -> IP[TIM5_IRQn] |= 0b1010; // (pp << (4 - g) | ((rp & (0xf >> g))& 0xf

  
  GPIOB -> ODR &= ~(0b1 << 5);
  USART1 -> DR = 'D';  
  USART1 -> DR = '\r';  
  USART1 -> DR = '\n'; 
  
  sendCharCom('D');
  sendCharCom('\r');
  sendCharCom('\n');
  while(1);
  return 0;
}

void sendCharCom(u8 chr)
{
  USART1 -> DR = chr;  
}

void TIM5_IRQHandler(void)
{
  u16 tsr ;
  tsr = TIM5 -> SR;
  if(t_counter >= 0xFF)
    goto reset;
  if(tsr & (0b1 << 0)) { // overflow
    ++ t_counter;
    goto next;
  }
  if(tsr & (0b1 << 1)) { // catched
    if (t_counter == 0) { // first
      ++ t_counter;
      TIM5 -> CNT = 0;
      TIM5 -> CCER |= 0b1 << 1;
      goto next;
    }
    else {
      goto reset;
    }
  }
  return;
  // send time via usart and reset TIM5
 reset:
  sendCharCom(t_counter);
  sendCharCom('\n');
  TIM5 -> CCER &= ~(0b1 << 1);
  // set interrupt for next time
 next:
  TIM5 -> SR = 0;
}
