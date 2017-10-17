/**
 * uasrt lower level
 * header
 */

#ifndef _USART_H_
#define _USART_H_
// includes
#include <stdint.h>
#include <firmware/stm32f10x_rcc.h>
#include <firmware/stm32f10x.h>
#include <firmware/stm32f10x_gpio.h>
#include <firmware/stm32f10x_usart.h>
#include <firmware/misc.h>
#include <firmware/stm32f10x_dma.h>

// types
typedef void (*usart_clk_f)(uint32_t,FunctionalState);
struct usart_lower_init_t {
  USART_TypeDef* USART_reg;      // register
  uint32_t       USART_baudrate; // baud rate
  uint32_t       USART_clk;      // USART's clock
  usart_clk_f    USART_clk_fun;  // init clock's function
};
struct usart_lower_nvic_t {
  USART_TypeDef* USART_NVIC_reg;    // register
  uint8_t        USART_NVIC_irq;    // IRQ
  uint8_t        USART_NVIC_pp;     // preemption priority
  uint8_t        USART_NVIC_sp;     // sub priority
  uint8_t        USART_NVIC_it_tc;     // IT bits
  uint8_t        USART_NVIC_it_rxne;     // IT bits
};
struct usart_gpio_init_t {
  GPIO_TypeDef* USART_GPIO_reg; // register
  uint16_t      USART_GPIO_rx;  // RX pin
  uint16_t      USART_GPIO_tx;  // TX pin
  uint32_t      USART_GPIO_clk;      // gpio's clock
  usart_clk_f   USART_GPIO_clk_fun;   // init clock's function
};
struct usart_dma_init_t {
  DMA_Channel_TypeDef* USART_DMA_tx_channel; // channel for DMA
  DMA_Channel_TypeDef* USART_DMA_rx_channel; // channel for DMA
  USART_TypeDef* USART_DMA_reg; // register
  uint32_t      USART_DMA_MAR_TX;            // memory address (TX)
  uint32_t      USART_DMA_MAR_RX;            // memory address (RX)
  uint32_t      USART_DMA_clk;            // DMA's clock
  usart_clk_f   USART_DMA_clk_fun;         // init clock's function
};
struct usart_dma_nvic_init_t {
  DMA_Channel_TypeDef* USART_DMA_NVIC_tx; // USART' DMA' tx channel
  DMA_Channel_TypeDef* USART_DMA_NVIC_rx; // USART' DMA' rx channel
  uint8_t  USART_DMA_NVIC_tx_irq;   // USART's DMA's NVIC's TX IRQ
  uint8_t  USART_DMA_NVIC_rx_irq;   // USART's DMA's NVIC's RX IRQ
  uint8_t  USART_DMA_NVIC_pp;     // preemption priority
  uint8_t  USART_DMA_NVIC_sp;     // sub priority
};
//functions
void usart_init(struct usart_lower_init_t);
void usart_gpio_init(struct usart_gpio_init_t);
void usart_nvic_init(struct usart_lower_nvic_t);
void usart_dma_init(struct usart_dma_init_t);
void usart_dma_nvic_init(struct usart_dma_nvic_init_t);
#endif // !_USART_H_
