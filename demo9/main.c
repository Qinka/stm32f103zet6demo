#include "stm32f10x.h"
#include "misc.h"
#include <stdbool.h>

int _exit(void) {
  return 0;
}

// usart
void sendChar(u8);
void sendStr(const u8*);
void sendHex(u8);

u16 recvChar(bool);

u8  RmtSta = 0;
u16 Dval;
u32 RmtRec = 0;
const u8* RmtRecA = &RmtRec;

u8 RmtCnt = 0;
u8 RmtBit = 32;

u32 TransBuf;
u8 TransCounter = 33;
bool TransBlock = false;

void sendNEC(u8,u8);
u16  recvNEC(bool is);

int main(void) {
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
  USART1 -> CR1 |= 0x200C; // 1 bit for stop, without parity

  // initialize NEC send/recv PA2/PA6 (TIM2_CH3/TIM3_CH1)
  RCC -> APB2ENR |= 0b1 << 2;
  GPIOA -> CRL &= ~(0xF << 8);
  GPIOA -> CRL |= 0b0011 << 8;
  GPIOA -> ODR |= 0b1 << 2;
  GPIOA -> CRL &= ~(0xF << 24);
  GPIOA -> CRL |= 0b1000 << 24;
  GPIOA -> ODR |= 0b1 << 6; // pull-up

  // initialize Timer
  // configure TIM2/3 clock
  RCC -> APB1ENR |= 0b11 << 0;
  // setting TIM2
  TIM2 -> ARR = 10;
  TIM2 -> PSC = 72 - 1;
  TIM2 -> DIER |= 0b1 << 0;
  TIM2 -> CR1 &= ~(0b1 << 0);
  // setting TM3 
  TIM3 -> ARR = 10000;
  TIM3 -> PSC = 72 - 1;
  TIM3 -> CCMR1 &= ~(0xFF) << 0;
  TIM3 -> CCMR1 |= 0b1 << 0;
  TIM3 -> CCER |= 0b11 << 0;
  TIM3 -> DIER |= 0b1 << 1;
  TIM3 -> DIER |= 0b1 << 0;
  TIM3 -> CR1 |= 0b1 << 0;


  // init interrupt for TIM2/3
  // nvic (group 2, p (3,1)/(1,3))
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitTypeDef NVIC_InitStructure2,NVIC_InitStructure3;
  NVIC_InitStructure2.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure2);
  NVIC_InitStructure3.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure3);

  sendStr("Done...\r\n");

  u32 oldR = RmtRec + 1;

  //while(1) sendNEC('A','B');

  while(1) {
    u16 uRecv = 0;
    u16 nRecv = 0;
    // send received char from usart1
    uRecv = recvChar(false);
    if(uRecv & 0xFF00)
      sendNEC(uRecv & 0xFF,uRecv & 0xFF);
    // display received data (from NEC)
    nRecv = recvNEC(false);
    if(nRecv & 0xFF00)
      sendChar(nRecv & 0xFF);
  }
  return 0;
}

void sendChar(u8 chr)
{
  while((USART1 -> SR & 0x40) == 0);
  USART1 -> DR = chr;
}

void sendHex(u8 chr) {
  char low,high;
  low = (chr & 0x0F) + '0';
  high = ((chr >> 4) & 0x0F) + '0';
  if (low > '9')
    low += 7;
  if (high > '9')
    high += 7;
  sendChar(high);
  sendChar(low);
}

void sendStr(const u8 * str)
{
  while(*str != '\0')
    sendChar(*str++);
}

/**
   @param is (false -> non-blocking)
 */
u16 recvChar(bool is) {
  u16 recv = 0;
  while(is && (USART1 -> SR & 0x20));
  if (USART1 -> SR & 0x20) {
    recv  = USART1 -> DR;
    recv |= (0b1 << 8);
  }
  return recv;
}


void TIM3_IRQHandler(void) {
  u16 tsr;
  tsr=TIM3->SR;
  if(tsr&0X01) { // overflow
    if(RmtSta&0x80) {
      RmtSta&=~0X10;
      if((RmtSta&0X0F)==0X00)
        RmtSta|=1<<6;
      if((RmtSta&0X0F)<14)
        RmtSta++;
      else {
        RmtSta&=~(1<<7);
        RmtSta&=0XF0;
      }
    }
  }	
  if(tsr&(1<<1)) { // cc1e interrupt
    if(GPIOA -> IDR & (0b1 << 6)) { // rising
      TIM3->CCER|=1<<1;
      TIM3->CNT=0;
      RmtSta|=0X10;
    }
    else { // failing
      Dval=TIM3->CCR1;
      TIM3->CCER&=~(1<<1);
      if(RmtSta&0X10) {
        if(RmtSta&0X80) { // catch guidance code
          if(Dval>300&&Dval<800) {
            RmtRec<<=1;
            RmtRec &= ~0b1;
          }
          else if(Dval>1400&&Dval<1800) { // standard is 1680(us)
            RmtRec<<=1; // left-shift
            RmtRec |= 0b1; // catch 1
          }
          else if(Dval>2200&&Dval<2600) { // key for 2500(us)
            RmtCnt++; // ++
            RmtSta&=0XF0; //clean
          }
        }
        else if(Dval > 4200&&Dval<4700) { // standard for 4500(us)
          RmtSta|=1<<7;
          RmtCnt=0;
        }
      }
      RmtSta&=~(1<<4);
    }
  }
  TIM3->SR=0; //clean
}

void TIM2_IRQHandler(void) {
  u16 tsr;
  tsr = TIM2 -> SR;
  if(tsr & 0x1) { // time to change
    if (TransCounter == 33) { // low level 9ms
      GPIOA -> ODR &= ~(0b1 << 2);
      TIM2 -> CNT = 0;
      TIM2 -> ARR = 4499; // 4500
      //TIM2 -> ARR = 8999; // 9000
      -- TransCounter;
    }
    else if(TransCounter == 32) { // high level 4.5ms
      GPIOA -> ODR |= 0b1 << 2;
      TIM2 -> CNT = 0;
      TIM2 -> ARR = 4499; // 4500
      -- TransCounter;
    }
    else if(TransCounter < 32 && TransCounter >= 0) { // send level
      if(GPIOA -> ODR & 0x4) { // to low
        GPIOA -> ODR &= ~(0b1 << 2);
        TIM2 -> CNT = 0;
        TIM2 -> ARR = 559; // 559
      }
      else { // to high
        GPIOA -> ODR |= 0b1 << 2;
        TIM2 -> CNT = 0;
        if (TransBuf & (0b1 << TransCounter)) // 1
          TIM2 -> ARR = 1679; // 1680
        else
          TIM2 -> ARR =  559; //  560
        -- TransCounter;
      }
    }
    else { // stop send
      if(GPIOA -> ODR & 0x4) { // to low
        GPIOA -> ODR &= ~(0b1 << 2);
        TIM2 -> CNT = 0;
        TIM2 -> ARR = 559; // 559
      }
      else { // to high
        TransCounter = 33;
        GPIOA -> ODR |= 0b1 << 2;
        TIM2 -> CR1 &= ~(0b1 << 0);
        TIM2 -> CNT = 0;
        TIM2 -> ARR = 10;
        TransBlock = false;
      }
    }
  }
  TIM2 -> SR &= ~(0b1 << 0);
}

void sendNEC(u8 addr,u8 data) {
  while(TransBlock);
  TransCounter = 33;
  TransBuf =0;
  TransBuf |= addr << 24;
  TransBuf |= (~addr & 0xFF) << 16;
  TransBuf |= data << 8;
  TransBuf |= (~data & 0xFF) << 0;
  TIM2 -> CNT = 0;
  TIM2 -> CR1 |= 0b1 << 0;
  TransBlock = true;
}

u16 recvNEC(bool is) {
  while(is && RmtSta&(1<<6));
  u16 recv = 0;
  if(RmtSta&(1<<6)) {
    recv |= RmtRec & 0xFF;
    recv |= 0b1 << 8;
  }
  RmtSta &= ~(0b1 << 6);
  //RmtRec = 0;
  RmtCnt = 0;
  return recv;
}
