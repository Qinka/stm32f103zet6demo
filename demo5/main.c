#include "stm32f10x.h"

int _exit(void)
{
  return 0;
}
int main(void)
{
  // serial for baud rate 115200
  // enable USART1's clock
  RCC -> APB2ENR |= 0b1 << 14;
  // configure GPIOA's port(used by usart)
  RCC -> APB2ENR |= 1<<2;
  GPIOA -> CRH &= 0XFFFFF00F;
  GPIOA -> CRH |= 0X000008B0;
  // reset USART1
  RCC -> APB2RSTR |=  0b1 << 14;
  RCC -> APB2RSTR &= ~(0b1 << 14);
  USART1 -> BRR = 0x0271; // for mantissa 0x27; faction 0x01;
  USART1 -> CR1 |= 0x202C; // 1 bit for stop, without parity, interrupt when receive buffer is not empty
  // init interrupt for USART1
  // nvic (group 2, p (2,2))
  u32 tmp = SCB -> AIRCR;
  tmp &= 0xF8FF;
  tmp |= 0x05FA << 16;
  tmp |= 0x0200; // ((~ 2) & 0b111) << 8
  SCB -> AIRCR = tmp;
  NVIC -> ISER[USART1_IRQn /32] |= (0b1 << USART1_IRQn % 32);
  NVIC -> IP[USART1_IRQn] |= 0b1010; // (pp << (4 - g) | ((rp & (0xf >> g))& 0xf
  USART1->DR='a';
  while((USART1->SR&0X40)==0);
  USART1->DR='\n';
  while((USART1->SR&0X40)==0);
  USART1->DR='\r';
  while(1);
  return 0;
}

void USART1_IRQHandler(void)
{
  u32 sr = USART1 -> SR;
  u32 dr;
  if(sr & (0b1 << 5)) {
    dr = USART1 -> DR;
    ++ dr;
    USART1 -> DR = dr;
  }
}
