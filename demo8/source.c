#include "stm32f10x.h"
#include "misc.h"

int _exit(void)
{
  return 0;
}
int main(void)
{
  // serial for baud rate 115200
  // enable USART2's clock
  RCC -> APB1ENR |= 0b1 << 17;
  // configure GPIOA's port(used by usart)
  RCC -> APB2ENR |= 1<<2;
  GPIOA -> CRL &= 0XFFFF00FF;
  GPIOA -> CRL |= 0X00008B00;
  // reset USART1
  RCC -> APB1RSTR |=  0b1 << 17;
  RCC -> APB1RSTR &= ~(0b1 << 17);
  USART2 -> BRR  = 0x0138; // for mantissa 0x13; faction 0x08;(19.5)
  USART2 -> CR1 |= 0x202C; // 1 bit for stop, without parity, interrupt when receive buffer is not empty
  // init interrupt for USART2
  // nvic (group 2, p (2,2))
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitTypeDef NVIC_InitStructure2;
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure2);


  while((USART2->SR&0X40)==0);
  USART2->DR='a';
  while((USART2->SR&0X40)==0);
  USART2->DR='\n';
  while((USART2->SR&0X40)==0);
  USART2->DR='\r';
  while(1);
  return 0;
}

void USART2_IRQHandler(void)
{
  u32 sr = USART2 -> SR;
  u32 dr;
  if(sr & (0b1 << 5)) {
    dr = USART2 -> DR;
    ++ dr;
    while((USART2->SR&0X40)==0);
    USART2 -> DR = dr;
  }
}
