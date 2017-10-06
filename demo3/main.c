#include "stm32f10x.h"
#include "config.h"
#include <string.h>

static u8 beep_buf[_BEEP_BUF_SIZE_] = {0};
u8* beep_cur = 0x0;
int8_t beep_counter = 0;

void beep_launch(void);
void beep_wait(void);
void bepp_unsafe(char *);

int _exit(void)
{
  return 0;
}

int main(void)
{
  // init GPIO
  // init clock                                     
  RCC -> APB2ENR |= (0b1 << _BEEP_GPIO_CLK_);
  // clean beep(PB0)
  _BEEP_GPIO_SET_ -> _BEEP_GPIO_CR_ &= ~ (0xF << _BEEP_GPIO_CRI_);
  _BEEP_GPIO_SET_ -> _BEEP_GPIO_CR_ |= 0b0011 << _BEEP_GPIO_CRI_;
  _BEEP_GPIO_SET_ -> ODR &= ~(0b1 << _BEEP_GPIO_PORT_);
  // init Timer 3(TIM3)
  RCC -> APB1ENR |= 0b1 << 1;
  TIM3 -> PSC = 7199; // 7.2K for 72MHz => 10KHz (1/10 ms)
  TIM3 -> ARR = _MORSE_SPEED_;
  TIM3 -> CR1  &= ~(0b1 << 0);
  TIM3 -> CR1 &= ~(0b1 << 0);
  // init for nvic (group 2, p (2,2))
  u32 tmp = SCB -> AIRCR;
  tmp &= 0xF8FF;
  tmp |= 0x05FA << 16;
  tmp |= 0x0200; // ((~ 2) & 0b111) << 8
  SCB -> AIRCR = tmp;
  NVIC -> ISER[TIM3_IRQn /32] |= (0b1 << TIM3_IRQn % 32);
  NVIC -> IP[TIM3_IRQn] |= 0b1010; // (pp << (4 - g) | ((rp & (0xf >> g))& 0xf
  /*
  beep_buf[0] = 0b10101010;
  beep_buf[1] = 0b11000100;
  beep_buf[2] = 0b10101110;
  beep_buf[3] = 0b11010001;
  beep_buf[4] = 0b10111010;
  beep_buf[5] = 0b11000111;
  beep_buf[6] = 0b10111011;
  beep_buf[7] = 0b11000000;
  beep_buf[8] = 0b10101110;
  beep_buf[9] = 0b11110001;
  beep_buf[10] = 0b11101110;
  beep_buf[11] = 0b11110001;
  beep_buf[12] = 0b10111010;
  beep_buf[13] = 0b10010111;
  beep_buf[14] = 0b10101000;
  beep_buf[15] = 0b11110101;
  beep_buf[16] = 0b00000000;
  beep_launch();  */
  beep_unsafe(_DONE_BEEP_STR_);
  while(1);
  return 0;
}

void TIM3_IRQHandler(void)
{
  if (TIM3 -> SR & 0b1) {
    if (*beep_cur) {
      if(*beep_cur & (0b1 << beep_counter))
	_BEEP_GPIO_SET_ -> ODR |=   0b1 << _BEEP_GPIO_PORT_;
      else
	_BEEP_GPIO_SET_ -> ODR &= ~(0b1 << _BEEP_GPIO_PORT_);
      -- beep_counter;
      if (beep_counter < 0) {
	++ beep_cur;
	beep_counter = 6;
      }
    }
    else {
      TIM3 -> CR1  &= ~(0b1 << 0);
      TIM3 -> DIER &= ~(0b1 << 0);
      _BEEP_GPIO_SET_ -> ODR &= ~(0b1 << _BEEP_GPIO_PORT_);
    }    
  }
  TIM3 -> SR &= ~(0b1 << 0);
}


void beep_launch()
{
  beep_cur = beep_buf;
  beep_counter = 6;
  TIM3 -> DIER |= 0b1 << 0;
  TIM3 -> CR1  |= 0b1 << 0;
}

void beep_wait(void)
{
  while(*beep_cur);
  for(u32 i =0; i < _BEEP_BUF_SIZE_; ++i)
    beep_buf[i] =0;
}

void beep_buf_set_unsafe(u8 item)
{
  if (item)
    *beep_cur |= 0b1 << beep_counter;
  else
    *beep_cur &= ~(0b1 << beep_counter);
  *beep_cur |= 0b10000000;
  -- beep_counter;
  if (beep_counter < 0) {
    ++ beep_cur;
    beep_counter = 6;
  }
}

void beep_unsafe (char *str)
{
  beep_cur = beep_buf;
  beep_counter = 6;
  while(*str != '\0') {
    switch (*str) {
    case 'A':
    case 'a':
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'B':
    case 'b':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'C':
    case 'c':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'D':
    case 'd':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1); 
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'E':
    case 'e':
      beep_buf_set_unsafe(1); // .
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'F':
    case 'f':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'G':
    case 'g':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'H':
    case 'h':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'I':
    case 'i':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'J':
    case 'j':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'K':
    case 'k':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'L':
    case 'l':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'M':
    case 'm':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'N':
    case 'n':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'O':
    case 'o':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'P':
    case 'p':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'Q':
    case 'q':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'R':
    case 'r':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'S':
    case 's':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'T':
    case 't':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'U':
    case 'u':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'V':
    case 'v':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'W':
    case 'w':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'X':
    case 'x':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'Y':
    case 'y':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case 'Z':
    case 'z':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '0':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '1':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '2':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '3':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '4':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '5':
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '6':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '7':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '8':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case '9':
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); // ---
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(1);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(1); //.
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0); // 2t
      beep_buf_set_unsafe(0);
      break;
    case ' ':      
      beep_buf_set_unsafe(0); // 4t
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0);
      beep_buf_set_unsafe(0);
      break;
    default:
      break;
    }
    ++ str;
  }
  ++beep_cur;
  *beep_cur = '\0';
  beep_cur = beep_buf;
  beep_counter = 6;
  beep_launch();
}
