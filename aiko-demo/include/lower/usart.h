/**
 * uasrt lower level
 * header
 */
#include <stdint.h>
#include <firmware/stm32f10x_rcc.h>

#ifndef _USART_H_
#define _USART_H_

// types
typedef void (*usart_clk_f)(uint32_t,FunctionalState);
struct usart_lower_init_t {
  uint32_t    USART_reg,      // register
  uint32_t    USART_baudrate, // baud rate
  uint32_t    USART_clk,      // USART's clock
  usart_clk_f USART_clk_fun   // init clock's function
};
struct usart_lower_nvic_t {
  uint32_t USART_NVIC_reg,    // register
  uint8_t  USART_NIIC_irq,    // IRQ
  uint8_t  USART_NIVC_pp,     // preemption priority
  uint8_t  USART_NVIC_sp,     // sub priority
  uint16_t USART_NVIC_it,     // IT bits
};
struct usart_gpio_init_t {
  GPIO_TypeDef* USART_GPIO_reg, // register
  uint16_t      USART_GPIO_rx,  // RX pin
  uint16_t      USART_GPIO_tx,  // TX pin
  uint32_t      USART_GPIO_clk,      // gpio's clock
  usart_clk_f   USART_GPIO_clk_fun   // init clock's function
};
struct usart_dma_init_t {
  DMA_Channel_TypeDef* USART_DMA_channel, // channel for DMA
  uint32_t      USART_DMA_MAR,            // memory address
  uint32_t      USART_DMA_PAR,            // peripheral address
  uint32_t      USART_DMA_dir,            // direction
  uint32_t      USART_DMA_clk,            // DMA's clock
  usart_clk_f   USART_DMA_clk_fun         // init clock's function
};
//functions
void usart_init(struct usart_lower_init_t);
void usart_gpio_init(struct usart_gpio_init_t);
void usart_nvic_init(struct usart_lower_nvic_t);
void usart_dma_init(struct usart_dma_init_t);
#endif // !_USART_H_
