/**
 * USART lower initialization
 * 
 */

 #define _USART_C_

#include <firmware/stm32f10x.h>
#include <firmware/stm32f10x_gpio.h>
#include <firmware/stm32f10x_usart.h>
#include <lower/usart.h>

/**
 *  usart initializing
 * @param uli the struct of USART's initialization
 */
void usart_init(struct usart_lower_init_t uli) {
  // should init the GPIO port first
  uli -> USART_clk_fun (uli -> USART_clk, ENABLE);
  USART_DeInit(uli -> USART_reg);
  USART_InitTypeDef USART_init_s = {
    USART_BaudRate: uli -> USART_baudrate,
    USART_WordLength: USART_WordLength_8b,
    USART_StopBits: USART_Parity_No,
    USART_HardwareFlowControl: USART_HardwareFlowControl_None,
    USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  };
  USART_Init(uli -> USART_reg,&USART_init_s);
}

void usart_nvic_init(struct usart_lower_nvic_t uln) {
  NVIC_InitTypeDef usart_nvic_init_s = {
    NVIC_IRQChannel: uln -> USART_NIIC_irq,
    NVIC_IRQChannelPreemptionPriority: uln -> USART_NIVC_pp,
    NVIC_IRQChannelSubPriority: uln -> USART_NVIC_sp,
    NVIC_IRQChannelCmd: ENABLE
  };
  NVIC_Init(&usart_nvic_init_s);
  USART_ITConfig(uln -> USART_NVIC_reg, uln -> USART_NVIC_it, ENABLE);
}

void usart_gpio_init(struct usart_gpio_init_t ugi) {
  ugi -> USART_GPIO_clk_fun(ugi -> USART_GPIO_clk, ENABLE);
  GPIO_InitTypeDef usart_gpio_init_s_rx = {
    GPIO_Pin: ugi -> USART_GPIO_rx,
    GPIO_GPIO_Mode: GPIO_Mode_IN_FLOATING
  }；
  GPIO_InitTypeDef usart_gpio_init_s_tx = {
    GPIO_Pin: ugi -> USART_GPIO_tx,
    GPIO_GPIO_Mode: GPIO_Mode_AF_PP,
    GPIO_Speed: GPIO_Speed_50MHz
  }；
  GPIO_Init(ugi -> USART_GPIO_reg, &usart_gpio_init_s_rx);
  GPIO_Init(ugi -> USART_GPIO_reg, &usart_gpio_init_s_tx);
}

void usart_dma_init(struct usart_dma_init_t udi) {
 udi -> USART_DMA_clk_fun(udi -> USART_DMA_clk, ENABLE);
 DMA_DeInit(udi -> USART_DMA_channel);
 DMA_InitTypeDef usart_dma_init_s {
  DMA_PeripheralBaseAddr: udi -> USART_DMA_PAR,
  DMA_MemoryBaseAddr: udi -> USART_DMA_MAR,
  DMA_DIR: DMA_DIR_PeripheralDST,
  DMA_PeripheralInc: DMA_PeripheralInc_Disable,
  DMA_MemoryInc: DMA_MemoryInc_Enable,
  DMA_PeripheralDataSize: DMA_PeripheralDataSize_Byte,
  DMA_MemoryDataSize: DMA_MemoryDataSize_Byte,
  DMA_Mode: DMA_Mode_Normal,
  DMA_Priority: DMA_Priority_Medium,
  DMA_M2M: DMA_M2M_Disable
 };
 DMA_Init(udi -> USART_DMA_channel, &usart_dma_init_s);
 DMA_Cmd(udi -> USART_DMA_channel, DISABLE);
 USART_Cmd(USART, ENABLE);    // error  
}