#include "stm32f10x.h"
#include "misc.h"

int _exit(void)
{
  return 0;
}
void sendChar(u8);
void semdStr(const u8*);

// for trans
int8_t t_counter=-2;
u32 code_buf;
const u8* code = &code_buf;
u8 bit_counter=32;



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
  u16 tsr ;
  tsr = TIM4 -> SR;
  if(tsr & 0b1) { //  overflow
    if(bit_counter == 32) { // sync-signal
      ++ t_counter;
      if(t_counter > 9) { // reset
	bit_counter = 32;
	t_counter = -2;	
	TIM4 -> CCER |= 0b1 << 13; // to failling
      }
    }
    else if(t_counter < 1) // rx for logist 1
      ++t_counter;
    else { // reset
      t_counter = -2;
      bit_counter = 32;
      TIM4 -> CCER |= 0b1 << 13; // to failling
    }
  }
  else if(tsr & 0b10000) { // failling or rising
    if (bit_counter == 32) {// sync-signal
      if(t_counter == 0) { // failling
	++ t_counter;
	TIM4 -> CCER &= ~(0b1 << 13); // to rising
      }
      else if(t_counter == 7) { // checked rising
	-- bit_counter;
	t_counter = -2;
      }
      else { // reset
	bit_counter = 32;
	t_counter = -2;
	TIM4 -> CCER |= 0b1 << 13; // to failling
      }
    }
    else if (bit_counter < 32 && bit_counter >=0) {// rx
      if(t_counter == -2) { // wave (failling)
	++ t_counter;
	TIM4 -> CCER &= ~(0b1 << 13); // to rising
      }
      else if(t_counter == -1) { // wave (rising)
	++ t_counter;
	TIM4 -> CCER |= 0b1 << 13; // to failling
      }	
      else if(t_counter < 2) { // failling
	code_buf &= ~(0b1 << bit_counter);
	code_buf |= (t_counter & 0b1) << bit_counter;
	TIM4 -> CCER &= ~(0b1 << 13); // to rising
	if(bit_counter == 0) { // final
	  if(code[0] == ~code[1] && code [2] == ~code[3]) { // right
	    sendChar(code[0]);
	    sendChar(code[1]);
	    sendChar(code[2]);
	    sendChar(code[3]);
	  }
	  else { // reset
	    bit_counter = 32;
	    t_counter = -2;
	    TIM4 -> CCER |= 0b1 << 13; // to failling
	  }
	}
	else {
	  -- bit_counter;
	  t_counter = -2;
	}
      }
      else { // reset
	bit_counter = 32;
	t_counter = -2;
	TIM4 -> CCER |= 0b1 << 13; // to failling
      }
    }
  }
}
