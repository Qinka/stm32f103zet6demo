#include "stm32f10x.h"
#include "misc.h"
#include "string.h"
#include "ANSI-term.h"

// exit
void (*_exit)(void) = 0;

void DMA_USART1_config(void);

void USART1_config(void);

void DMA_USART1_send(u8 *);

static u8 isfinish = 0;

char termbuf[4096] = "";


int main(void) {
  void * chann = DMA1_Channel4;
  int i = 1;
  // enable the usart1 and DMA
  USART1_config();
  DMA_USART1_config();


  while((USART1 -> SR & 0x40) == 0);
  USART1 -> DR = '1';
  DMA_USART1_send(HIDE_CUR);
  while(1){
    DMA_USART1_send(FONT_COLOR(COLOR_GREEN) \
                    "123" "455\r\n" \
                    FONT_COLOR(COLOR_DEFAULT));
    for(int i=0xFFFFFF;i>0;--i);
  }
}


/**
 * Use the DMA1
 * channel 4 for send data
 */
void DMA_USART1_config(void) {
  // enable the DMA's clock
  RCC -> AHBENR |= 0b1 << 0;
  // configure the DMA
  DMA1_Channel4 -> CCR = 0x0;
  DMA1_Channel4 -> CPAR = (u32) & USART1 -> DR;
  DMA1_Channel4 -> CMAR = 0;
  DMA1_Channel4 -> CNDTR = 0;
  DMA1_Channel4 -> CCR |= 0b1 << 4; // read from memory
  DMA1_Channel4 -> CCR &= ~(0b1 << 5); // normal mode
  DMA1_Channel4 -> CCR &= ~(0b1 << 6); // the peripheral's address do not change
  DMA1_Channel4 -> CCR |= 0b1 << 7; // the memory's address need to be added 1
  DMA1_Channel4 -> CCR &= ~(0b101 << 8); // width of both peripheral and memory's data are 8 bits
  DMA1_Channel4 -> CCR |= 0b1 << 12; // middle priority
  DMA1_Channel4 -> CCR &= ~(0b1 << 14); // non mem-to-mem mode
  DMA1_Channel4 -> CCR |= 0b111 << 1; // for interrupt when TC
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

void USART1_config(void) {
  // initialize usart1
  // configure the PA9 & PA10's clock
  RCC -> APB2ENR |= 0b1 << 2;
  // configure usart1's clock
  RCC -> APB2ENR |= 0b1 << 14;
  // configure PA9 & PA10
  GPIOA -> CRH &= ~(0xFF << 4);
  GPIOA -> CRH |= 0x8B << 4;
  // reset usart1
  RCC -> APB2RSTR |=  0b1 << 14;
  RCC -> APB2RSTR &= ~(0b1 << 14);
  // configure usart 115200 & (without interrupt)
  USART1 -> BRR = 0x0271; // for mantissa 0x27; faction 0x01;
  USART1 -> CR3 |= 0b1 << 7;
  USART1 -> CR1 |= 0x200C; // 1 bit for stop, without parity
}

void DMA_USART1_send(u8 *data){
  // block until done
  while(isfinish);
  isfinish = 1;
  // disable it
  DMA1_Channel4 -> CCR &= ~(0b1 << 0);
  DMA1_Channel4 -> CMAR = (u32)data;
  DMA1_Channel4 -> CNDTR = strlen(data);
  DMA1_Channel4 -> CCR |= 0b1 << 0;
}

void DMA1_Channel4_IRQHandler(void) {
  if(DMA1 -> ISR & (0b1 << 13))
    isfinish = 0;
  DMA1 -> IFCR |= ~0;
}
