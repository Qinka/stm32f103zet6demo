/**
 * test for usart
 */

#define _USART_TEST_C_
#include <func_test/usart_test.h>
#include <aiko/job/job_s.h>

char tx_buf;
char rx_buf;
char dma_tx_buf[USART_TEST_TX_BUG_SIZE];
char dma_rx_buf[USART_TEST_RX_BUG_SIZE];
struct job_t usart_test_send_new_j;
void usart_test_send_new_f(struct job_t*);


void usart_test_init() {
  struct usart_lower_init_t uli_s = {
    .USART_reg = USART_TEST_USART,
    .USART_baudrate = USART_TEST_BaudRate,
    .USART_clk = USART_TEST_CLK,
    .USART_clk_fun = USART_TEST_CLK_FUN
  };
  struct usart_lower_nvic_t ulni_s = {
    .USART_NVIC_reg = USART_TEST_USART,
    .USART_NVIC_irq = USART_TEST_IRQ,
    .USART_NVIC_pp = USART_TEST_PP,
    .USART_NVIC_sp = USART_TEST_SP,
    .USART_NVIC_it_tc = 1,
    .USART_NVIC_it_idle = 1,
  };
  struct usart_gpio_init_t ugi_s = {
    .USART_GPIO_reg = USART_TEST_PORT,
    .USART_GPIO_rx = USART_TEST_RX_PIN,
    .USART_GPIO_tx = USART_TEST_TX_PIN,
    .USART_GPIO_clk = USART_TEST_PORT_CLK,
    .USART_GPIO_clk_fun = USART_TEST_PORT_CLK_FUN
  };
  struct usart_dma_init_t udi_s = {
    .USART_DMA_tx_channel = USART_TEST_TX_CH,
    .USART_DMA_rx_channel = USART_TEST_RX_CH,
    .USART_DMA_reg = USART_TEST_USART,
    .USART_DMA_MAR_TX = dma_tx_buf,
    .USART_DMA_MAR_RX = dma_rx_buf,
    .USART_DMA_clk = USART_TEST_DMA_CLK,
    .USART_DMA_clk_fun = USART_TEST_DMA_CLK_FUN
  };
  struct usart_dma_nvic_init_t udni_s = {
    .USART_DMA_NVIC_tx = USART_TEST_TX_CH,
    .USART_DMA_NVIC_rx = USART_TEST_RX_CH,
    .USART_DMA_NVIC_tx_irq = USART_TEST_DN_TX_IRQ,
    .USART_DMA_NVIC_rx_irq = USART_TEST_DN_RX_IRQ,
    .USART_DMA_NVIC_pp = USART_TEST_DN_PP,
    .USART_DMA_NVIC_sp = USART_TEST_DN_SP,
  };
  usart_gpio_init(ugi_s);
  usart_init(uli_s);
  usart_dma_init(udi_s);
  usart_nvic_init(ulni_s);
  usart_dma_nvic_init(udni_s);
  USART_Cmd(USART_TEST_USART, ENABLE);
}


struct job_t usart_test_send_new_j = {
  .job_method = usart_test_send_new_f,
  .job_data_p = 0,
  .job_data_s = 0,
  .next_job_p = 0
};

void usart_test_send_new_f(struct job_t* j) {
  DMA_Cmd(USART_TEST_RX_CH,DISABLE);
  uint16_t len = USART_TEST_RX_BUG_SIZE - DMA_GetCurrDataCounter(USART_TEST_RX_CH);
  for(uint16_t i = 0; i < len; ++i)
    dma_tx_buf[i] = dma_rx_buf[i] + 1;
  DMA_Cmd(USART_TEST_TX_CH,DISABLE);
  DMA_SetCurrDataCounter(USART_TEST_TX_CH,len);
  DMA_Cmd(USART_TEST_TX_CH,ENABLE);
  DMA_ClearFlag(USART_RX_DMA_FG);
  DMA_SetCurrDataCounter(USART_TEST_RX_CH,USART_TEST_RX_BUG_SIZE);
  DMA_Cmd(USART_TEST_RX_CH,ENABLE);
}

void USART_TEST_USART_IRQHandler(void) {
  if(USART_GetITStatus(USART_TEST_USART,USART_IT_IDLE) != RESET) {
    job_add(&usart_test_send_new_j);
    uint32_t tmp;
    tmp = USART_TEST_USART -> SR;
    tmp = USART_TEST_USART -> DR;
  }
  USART_ClearITPendingBit(USART_TEST_USART,USART_IT_TC);
  USART_ClearITPendingBit(USART_TEST_USART,USART_IT_IDLE);
}

void USART_TEST_DMA_RX_IRQHandler(void) {
  DMA_ClearITPendingBit(USART_RX_DMA_IT);
  DMA_Cmd(USART_TEST_RX_CH, DISABLE);
  DMA_SetCurrDataCounter(USART_TEST_RX_CH,USART_TEST_RX_BUG_SIZE);
  DMA_Cmd(USART_TEST_RX_CH, ENABLE);
}

void USART_TEST_DMA_TX_IRQHandler(void)
{
  DMA_ClearITPendingBit(USART_TX_DMA_IT);
  DMA_Cmd(USART_TEST_TX_CH, DISABLE);
}