// media
// for translate

#include "stm32f10x.h"
#include "misc.h"

int _exit(void) {
}

int main(void) {
  // initialize USART and clock
  
  // initialize USART1 (PA9, PA10)
  RCC -> APB2ENR |= 0b1 << 2;
  RCC -> APB2ENR |= 0b1 << 14;
  GPIOA -> CRH &= ~(0xFF << 4);
  GPIOA -> CRH |= 0x8B << 4;
  RCC -> APB2RSTR |= 0b1 << 14;  
  RCC -> APB2RSTR &= ~(0b1 << 14);
  USART1 -> BRR = 0x0271;
  USART1 -> CR1 |= 0x202C;
  
  // initialize USART2(PA2,PA3)
  RCC -> APB1ENR |= 0b1 << 17;
  GPIOA -> CRL &= ~(0xFF << 8);
  GPIOA -> CRL |= 0x8B << 8;  
  RCC -> APB1RSTR |= 0b1 << 17;  
  RCC -> APB1RSTR &= ~(0b1 << 17);  
  USART2 -> BRR  = 0x0138; // for mantissa 0x13; faction 0x08;(19.5)
  USART2 -> CR1 |= 0x202C;

  // initialize NVIC
  
  // (usart1)nvic (group 2, p (1,2))
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitTypeDef NVIC_InitStructure1, NVIC_InitStructure2;
  NVIC_InitStructure1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure1);
  NVIC_Init(&NVIC_InitStructure2);

  while((USART1->SR&0X40)==0);
  USART1->DR='a';
  while((USART1->SR&0X40)==0);
  USART1->DR='\n';
  while((USART1->SR&0X40)==0);
  USART1->DR='\r';


  // empty loop
  while(1);
  
  return 0;
}


void USART1_IRQHandler(void)
{
  if(USART1 -> SR & (0b1 << 5)) {
    while((USART2 -> SR & 0x40) == 0);    
    USART2 -> DR = USART1 -> DR;
  }
}


void USART2_IRQHandler(void)
{
  if(USART2 -> SR & (0b1 << 5)) {
    while((USART1 -> SR & 0x40) == 0);    
    USART1 -> DR = USART2 -> DR;
  }
}
