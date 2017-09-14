#include "stm32f10x.h"
#include "misc.h"

int _exit(void)
{
  return 0;
}
void sendChar(u8);
void semdStr(const u8*);

// for trans
u8 RmtSta=0;
u16 Dval; // failling counter
u32 RmtRec=0; // data
const u8* RmtRecA = &RmtRec;
u8 RmtCnt=0; //pass


int main(void)
{
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

  // initialize infrared led reciver(PB9)
  RCC -> APB2ENR |= 0b1 << 3;
  GPIOB -> CRH &= ~(0xF << 4);
  GPIOB -> CRH |= 0b1000 << 4;
  GPIOB -> ODR |= 1 << 9;
  
  // initialize Timer
  // configure TIM4 clock
  RCC -> APB1ENR |= 0b1 << 2;
  // setting TIM4
  TIM4 -> ARR = 999;
  TIM4 -> PSC = 72 - 1;
  TIM4 -> CCMR2 &= ~(0xFF) << 8;
  TIM4 -> CCMR2 |= 0b1 << 8;
  TIM4 -> CCER |= 0b11 << 12;
  TIM4 -> DIER |= 0b1 << 4;
  TIM4 -> DIER |= 0b1 << 0;
  TIM4 -> CR1 |= 0b1 << 0;
  // init interrupt for TIM4
  // nvic (group 2, p (2,2))
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  


  sendStr("Done...\r\n");  
  while(1);
  return 0;
}

void sendChar(u8 chr)
{
  while((USART1 -> SR & 0x40) == 0);
  USART1 -> DR = chr;
}

void sendStr(const u8 * str)
{
  while(*str != '\0')
    sendChar(*str++);
}

void TIM4_IRQHandler(void)
{
  u16 tsr;
  tsr=TIM4->SR;
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
	sendChar(RmtRecA[0]);
	sendChar(RmtRecA[1]);
	sendChar(RmtRecA[2]);
	sendChar(RmtRecA[3]);
      }
    }
  }	
  if(tsr&(1<<4)) { // cc41e interrupt
    if(RDATA) { // rising
      TIM4->CCER|=1<<13;
      TIM4->CNT=0;
      RmtSta|=0X10;
    }
    else { // failling
      Dval=TIM4->CCR4;
      TIM4->CCER&=~(1<<13);
      if(RmtSta&0X10) {
	if(RmtSta&0X80) { // catch guidance code
	  if(Dval>300&&Dval<800) {
	    RmtRec<<=1;
	    RmtRec|=0;
	  }
	  else if(Dval>1400&&Dval<1800) { // standard is 1680(us)
	    RmtRec<<=1; // left-shift
	    RmtRec|=1; // catch 1
	  }
	  else if(Dval>2200&&Dval<2600) { // key for 2500(us)
	    RmtCnt++; // ++
	    RmtSta&=0XF0; //clean
	  }
	}
      }
    }
    else if(Dval > 4200&&Dval<4700) { // standard for 4500(us)
      RmtSta|=1<<7;
      RmtCnt=0;
      RmtSta&=~(1<<4);
    }
  }
  TIM4->SR=0; //clean
}
