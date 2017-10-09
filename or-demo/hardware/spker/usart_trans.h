/**
 */

#ifndef _USART_TRANS_H_
#define _USART_TRANS_H_

#include "frame.h"
#include "type.h"

// config
// USART's ports and pin
#define USART_GPIO_PORT  GPIOA
#define USART_GPIO_CLK_EN(x) RCC_APB2PeriphClockCmd(x,ENABLE)
#define USART_GPIO_CLK   RCC_APB2Periph_GPIOA
#define USART_CLK_EN(x) RCC_APB1PeriphClockCmd(x,ENABLE)
#define USART_CLK        RCC_APB1Periph_USART2
#define USART_RX_PIN     GPIO_Pin_3
#define USART_TX_PIN     GPIO_Pin_2
#define USART            USART2
// USART IRQn
#define USART_IRQ USART2_IRQn
// USART IRQ Handler
#define USART_IRQHandler USART2_IRQHandler
// USART DMA
#define USART_TX_DMA_CHANNEL DMA1_Channel7
#define USART_RX_DMA_CHANNEL DMA1_Channel6
// USART DMA IRQn
#define USART_TX_DMA_CHANNEL_IRQn DMA1_Channel7_IRQn
#define USART_RX_DMA_CHANNEL_IRQn DMA1_Channel6_IRQn
// USART DMA IRQ Handler
#define USART_TX_DMA_CHANNEL_IRQHandler DMA1_Channel7_IRQHandler
#define USART_RX_DMA_CHANNEL_IRQHandler DMA1_Channel6_IRQHandler
// USART DMA IRQ IT Status
#define USART_TX_DMA_IT DMA1_IT_TC7
#define USART_RX_DMA_IT DMA1_IT_TC6

void trans_config(void);
int send_recv(void *send_buf, short sbuf_size, void *recv_buf);

int send_frame_asyn(struct spker_frame frame,struct callback_task task);
int recv_frame_no_wait();
void loopwait(void*,int);


#endif // _USART_TRANS_H_
