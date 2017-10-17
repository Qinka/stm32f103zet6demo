/**
 * Test for USART
 */

 #ifndef _USART_TEST_H_
 // includes
 #include <lower/usart.h>

 #define USART_TEST_USART    USART1
 #define USRAT_TEST_BaudRate 115200
 #define USART_TEST_CLK      RCC_APB2Periph_USART1
 #define USART_TEST_CLK_FUN  RCC_APB2PeriphClockCmd
 
 #define USART_TEST_IRQ      USART1_IRQn
 #define USART_TEST_PP       2
 #define USART_TEST_SP       2
 #define USART_TEST_IT       (USART_IT_TC | USART_IT_RXNE)
 
 #define USART_TEST_PORT     GPIOA
 #define USART_TEST_RX_PIN   9
 #define USART_TEST_TX_PIN   10
 #define USART_TEST_PORT_CLK RCC_APB2Periph_GPIOA
 #define USART_TEST_PORT_CLK_FUN RCC_APB2PeriphClockCmd

 #define USART_TEST_RX_CH    DMA1_Channel5
 #define USART_TEST_TX_CH    DMA1_Channel4
 #define USART_TEST_DMA_CLK  RCC_AHBPeriph_DMA1
 #define USART_TEST_DMA_CLK_FUN RCC_AHBPeriphClockCmd

 #define USART_TSET_DN_TX_IRQ DMA1_Channel4_IRQ
 #define USART_TEST_DN_RX_IRQ DMA1_Channel5_IRQ

 #define USART_TEST_DN_PP 2
 #define USART_TEST_DN_SP 2
 #endif // !_USART_TEST_H_