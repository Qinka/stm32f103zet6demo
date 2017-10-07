#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "codetab.h"
#include "irda.h"

void (*_exit)(void) = 0;

int main(void) {
	unsigned char i;

  // config

  delayInit();
  NVIC_SetPriority (SysTick_IRQn, 0);
  IRDA_init();
	OLED_Init();
  OLED_CLS();
  OLED_6x8Str(0,0,"Done...");
  u32 data=0xABCD;
  const char *ds = &data;
  while(1){
    for(int i = 0; i < 4; ++i)
      OLED_6x8Hex(i*12,1,ds[i]);
    data = getCode();
  }
  return 0;
}
