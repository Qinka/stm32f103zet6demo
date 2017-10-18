/**
 * Test for USART
 */

#ifndef _USART_TEST_H_
// includes
#include <lower/usart.h>

#define USART_TEST_RX_BUG_SIZE 256
#define USART_TEST_TX_BUG_SIZE 256

#define USART_TEST_USART    USART2
#define USART_TEST_BaudRate 115200
#define USART_TEST_CLK      RCC_APB1Periph_USART2
#define USART_TEST_CLK_FUN  RCC_APB1PeriphClockCmd

#define USART_TEST_IRQ      USART2_IRQn
#define USART_TEST_PP       2
#define USART_TEST_SP       2

#define USART_TEST_PORT     GPIOA
#define USART_TEST_TX_PIN   GPIO_Pin_2
#define USART_TEST_RX_PIN   GPIO_Pin_3
#define USART_TEST_PORT_CLK RCC_APB2Periph_GPIOA
#define USART_TEST_PORT_CLK_FUN RCC_APB2PeriphClockCmd

#define USART_TEST_RX_CH    DMA1_Channel6
#define USART_TEST_TX_CH    DMA1_Channel7
#define USART_TEST_DMA_CLK  RCC_AHBPeriph_DMA1
#define USART_TEST_DMA_CLK_FUN RCC_AHBPeriphClockCmd

#define USART_TEST_DN_TX_IRQ DMA1_Channel7_IRQn
#define USART_TEST_DN_RX_IRQ DMA1_Channel6_IRQn
#define USART_TEST_DN_PP 2
#define USART_TEST_DN_SP 2

#define USART_TEST_USART_IRQHandler USART2_IRQHandler 
#define USART_TEST_DMA_TX_IRQHandler DMA1_Channel7_IRQHandler
#define USART_TEST_DMA_RX_IRQHandler DMA1_Channel6_IRQHandler
#define USART_TX_DMA_IT (DMA1_IT_TC7 | DMA1_IT_TE7)
#define USART_RX_DMA_IT (DMA1_IT_TC6 | DMA1_IT_TE6)
#define USART_RX_DMA_FG (DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6)

// for init
void usart_test_init(void);

#endif // !_USART_TEST_H_