#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "codetab.h"
#include "irda.h"
#include "type.h"
#include "frame.h"
#include "usart_trans.h"

void (*_exit)(void) = 0;
char play = 1;
extern char* usart_rx_buf;

int main(void) {
	unsigned char i;

  // config

  delayInit();
  NVIC_SetPriority (SysTick_IRQn, 0);
  IRDA_init();
  trans_config();
	OLED_Init();
  OLED_CLS();
  OLED_6x8Str(0,0,"Done...");
  u32 data=0xABCD;
  const char *ds = &data;
  int spk_rt = 0;
  short spk_code[] = {
    0xEF01,
    0xEF02,
    0xEF05,
    0xEF06
  };
  int spk_code2[] = {
    0x00EF
  };
  struct spker_frame play_f;
  struct spker_frame stop_f;
  struct spker_frame up_f;
  struct spker_frame down_f;
  fill_spker_frame(&play_f,&spk_code[0],2);
  fill_spker_frame(&stop_f,&spk_code[1],2);
  fill_spker_frame(&up_f,&spk_code[2],2);
  fill_spker_frame(&down_f,&spk_code[3],2);
  struct callback_task loopw = {
  callback_task_func:      &loopwait,
  callback_task_data:      0,
  callback_task_data_size: 0,
  };
  char spk_buf[16] = {0};
  while(1){
    if(recv_frame_no_wait() == 0) {
      for(int i = 0; i  < (int)usart_rx_buf[1] + 2; ++i ){
        OLED_6x8Hex(12*i,3,usart_rx_buf[i]);
      }
    }
    for(int i = 0; i < 4; ++i)
      OLED_6x8Hex(i*12,1,ds[i]);
    data = getCode();
    if (data == 0x00FFA857) {
        spk_rt = send_frame_asyn(play_f,loopw); //send_recv(stop_code,5,spk_buf);
    }
    else if (data == 0x00FFC23D) {
        spk_rt = send_frame_asyn(stop_f,loopw);// send_recv(play_code,5,spk_buf);
        play = 1;
    }
    else if(data == 0x00FF02FD) {
      spk_rt = send_frame_asyn(up_f,loopw);
    }
    else if(data == 0x00FF9867) {
      spk_rt = send_frame_asyn(down_f,loopw);
    }
    else if(data == 0x00FFA25D) {
      spk_rt = send_frame_asyn(down_f,loopw);
    }
    OLED_6x8Str(0,2,"SPK_RT:");
    if (spk_rt == -2)
      OLED_6x8Str(6*8,2,"END_ERROR");
    else if (spk_rt == -1)
      OLED_6x8Str(6*8,2,"BEGIN_ERR");
    else
      OLED_6x8Hex(6*8,2,spk_rt);
  }
  return 0;
}
