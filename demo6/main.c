#include "stm32f10x.h"
#include "misc.h"

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
  RCC -> APB2ENR |= 0b1 << 14;
  
  // setting GPIOA IO
  GPIOA -> CRH &= 0XFFFFF00F;
  GPIOA -> CRH |= 0X000008B0;
  // reset USART1
  RCC -> APB2RSTR |=  0b1 << 14;
  RCC -> APB2RSTR &= ~(0b1 << 14);
  // enable USART1's clock
  RCC -> APB2ENR |= 0b1 << 14;
  // config usart1
  USART1 -> BRR = 0x0271; // for mantissa 0x27; faction 0x01;
  USART1 -> CR1 |= 0x200C; // 1 bit for stop, without parity
  

  
  // configure TIM5 clock
  RCC -> APB1ENR |= 0b1 << 3;
  // setting PA0
  GPIOA -> CRL &= ~(0xF << 0);
  GPIOA -> CRL |= 0b1000 << 0;
  //GPIOA -> ODR &= ~(0b1 << 0); // pull down
  GPIOA -> BRR |= (0b1 << 0);
  // setting TIM5
  TIM5 -> ARR = 0xFFFF;
  TIM5 -> ARR = 72 - 1;
  TIM5 -> CCMR1 |= 0b1 << 0;
  TIM5 -> CCMR1 &= ~(0b1 << 4);
  TIM5 -> CCMR1 &= (0b1 << 10);
  TIM5 -> CCER &= ~(0b1 << 1);
  TIM5 -> CCER |= 0b1 << 0;
  TIM5 -> DIER |= 0b11 << 0;
  TIM5 -> CR1 |= 0b1 << 0;
  // init interrupt for TIM5
  // nvic (group 2, p (2,2))
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  

  
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
  while((USART1 -> SR & 0x40) == 0);
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
    if (t_counter = 0) { // first
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
  sendCharCom('\r');
  sendCharCom('\n');
  t_counter = 0;
  TIM5 -> CCER &= ~(0b1 << 1);
  // set interrupt for next time
 next:
  TIM5 -> SR &= ~(0b11 <<0);
}
