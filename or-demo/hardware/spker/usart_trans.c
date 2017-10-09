#include "usart_trans.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"


// global vars
struct callback_task send_frame_callback;
char usart_rx_buf[258];
const char* usart_dma_rx_buf = &usart_rx_buf[2];
char* usart_rx_cur = usart_rx_buf;
//char usart_dma_rx_buf[256];
char usart_dma_tx_buf[256];
volatile char usart_dma_tx_wait = 1;
#define wait_usart_dma_tx while(usart_dma_tx_wait);usart_dma_tx_wait=1;
volatile char usart_dma_rx_wait = 1;
#define wait_usart_dma_rx while(usart_dma_rx_wait);usart_dma_rx_wait=1;
volatile char usart_tx_wait = 1;
#define wait_usart_tx while(usart_tx_wait);usart_tx_wait=1;
volatile char usart_rx_wait = 1;
#define wait_usart_rx while(usart_rx_wait);usart_rx_wait=1;


// usart send global callback

// init for this module

void trans_config(void) {
  usart_config();
  dma_config();
  nvic_dma_config();
  nvic_usart_config();
}


/**
 * config the USART for speaking module
 */
void usart_config(void) {
  GPIO_InitTypeDef GPIO_InitStructure,GPIO_InitStructure2;
  USART_InitTypeDef USART_InitStructure;
  USART_GPIO_CLK_EN(USART_GPIO_CLK);
  USART_CLK_EN(USART_CLK);  //使能USART2，GPIOA时钟
  //USART1_TX   PA.2/3
  GPIO_InitStructure.GPIO_Pin = USART_TX_PIN; //PA.9复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);   //初始化GPIOA.0 发送端
  //USART1_RX   PA.10浮空输入
  GPIO_InitStructure2.GPIO_Pin = USART_RX_PIN;
  GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure2);   //初始化GPIOA.10接收端
  //USART 初始化设置
  USART_InitStructure.USART_BaudRate = 9600;//波特率设置;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;   //无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
  USART_Init(USART, &USART_InitStructure);  //初始化串口
  USART_Cmd(USART, ENABLE);                    //使能串口
}

void nvic_usart_config(void) {
  // 使能串口接收中断
  USART_ITConfig(USART, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART, USART_IT_TC, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void nvic_dma_config(void) {
  DMA_ITConfig(USART_TX_DMA_CHANNEL,DMA_IT_TC,ENABLE);
  DMA_ITConfig(USART_RX_DMA_CHANNEL,DMA_IT_TC,ENABLE);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART_TX_DMA_CHANNEL_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = USART_RX_DMA_CHANNEL_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void dma_config(void) {
  DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  // enable DMA for send
  DMA_DeInit(USART_TX_DMA_CHANNEL);
  DMA_InitStructure.DMA_PeripheralBaseAddr = &USART -> DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = usart_dma_tx_buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize =
    DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(USART_TX_DMA_CHANNEL, &DMA_InitStructure);
  //使能DMA
  // enable DMA for receive
  // 设置 DMA 源地址:串口数据寄存器地址*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = & USART -> DR;
  // 内存地址(要传输的变量的指针)
  DMA_InitStructure.DMA_MemoryBaseAddr = usart_dma_rx_buf;
  // 方向:从内存到外设
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  // 外设地址不增
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 内存地址自增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  // 外设数据单位
  DMA_InitStructure.DMA_PeripheralDataSize =
    DMA_PeripheralDataSize_Byte;
  // 内存数据单位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  // DMA 模式，一次或者循环模式
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
  // 优先级:中
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  // 禁止内存到内存的传输
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  //配置DMA通道
  DMA_Init(USART_RX_DMA_CHANNEL, &DMA_InitStructure);
  //使能DMA
  //DMA_Cmd (USART_RX_DMA_CHANNEL,ENABLE);
}

/**
 * send datas and receive datas
 * @param send_buf the buffer to be send
 * @param sbuf_size the buffer size to be send in bytes
 * @param recv_buf the buffer to storaged in (It is a good idea that
 * the size of this buffer is larger than 258(0xFF + 3))
 * @return the size of recv
 *  -1 : do not recv 0x7E
 */
int send_recv(void *send_buf, short sbuf_size, void *recv_buf) {
  char  * buf = send_buf;
  for (int i =0; i< sbuf_size; ++ i) {
    USART_SendData(USART,buf[i]);
    while(USART_GetFlagStatus(USART,USART_FLAG_TC) != SET);
  }
  buf = recv_buf;
  while(USART_GetFlagStatus(USART,USART_FLAG_RXNE) != SET);
  buf[0] = USART_ReceiveData(USART);
  if (buf[0] != 0x7E)
    return -1; //
  while(USART_GetFlagStatus(USART,USART_FLAG_RXNE) != SET);
  buf[1] = USART_ReceiveData(USART);
  for (u16 i = 0; i < buf[1] - 1; ++i) {
    while(USART_GetFlagStatus(USART,USART_FLAG_RXNE) != SET);
    buf[2+i] = USART_ReceiveData(USART);
  }
  while(USART_GetFlagStatus(USART,USART_FLAG_RXNE) != SET);
  buf[0] = USART_ReceiveData(USART);
  if (buf[buf[1]+1] != 0xEF)
    return -2; //
  return buf[1];
}


/**
 * the handler for usart interrupt
 */
void USART_IRQHandler(void) {
  if(USART_GetITStatus(USART,USART_IT_TC) != RESET) {
    usart_tx_wait = 0;
    USART_ClearITPendingBit(USART,USART_IT_TC);
  }
  if (USART_GetITStatus(USART,USART_IT_RXNE) != RESET) {
    usart_rx_wait = 0;
    char rx_d;
    rx_d = USART_ReceiveData(USART);
    if(rx_d == 0x7E) {
      usart_rx_cur = usart_rx_buf;
      *usart_rx_cur = rx_d;
      ++ usart_rx_cur;
    }
    else if(usart_rx_cur == usart_rx_buf + 1) {
      *usart_rx_cur = rx_d;
      ++ usart_rx_cur;
      USART_DMACmd(USART,USART_RX_DMA_CHANNEL,DISABLE);
      DMA_Cmd(USART_RX_DMA_CHANNEL,DISABLE);
      DMA_SetCurrDataCounter(USART_RX_DMA_CHANNEL,rx_d);
      DMA_Cmd(USART_RX_DMA_CHANNEL,ENABLE);
      USART_DMACmd(USART,USART_RX_DMA_CHANNEL,ENABLE);
    }
    USART_ClearITPendingBit(USART,USART_IT_RXNE);
  }
}

/**
 * the handler for (dma) usart rx interrupt handler
 */
void USART_RX_DMA_CHANNEL_IRQHandler(void) {
  if(DMA_GetITStatus(USART_RX_DMA_IT) != RESET)
    usart_dma_rx_wait = 0;
  DMA_ClearITPendingBit(USART_RX_DMA_IT);
}

/**
 * the handler for (dma) usart tx interrupt handler
 */
void USART_TX_DMA_CHANNEL_IRQHandler(void) {
  if(DMA_GetITStatus(USART_TX_DMA_IT) != RESET)
    usart_dma_tx_wait = 0;
  DMA_ClearITPendingBit(USART_TX_DMA_IT);
  usart_rx_cur = usart_rx_buf;
}


/**
 * send a frame
 */
int send_frame_asyn(struct spker_frame frame,struct callback_task task) {
  if (frame.frame_data[frame.frame_size - 1] != 0xEF) {
    frame.frame_data[frame.frame_size] = 0xEF;
    frame.frame_size ++;
  }
  for(int i = 0; i < frame.frame_size; ++i)
    usart_dma_tx_buf[i] = frame.frame_data[i];
  USART_DMACmd(USART,USART_TX_DMA_CHANNEL,DISABLE);
  DMA_Cmd(USART_TX_DMA_CHANNEL,DISABLE);
  // 传输大小
  DMA_SetCurrDataCounter(USART_TX_DMA_CHANNEL,frame.frame_size);
  USART_SendData(USART,0x00);
  wait_usart_tx;
  USART_SendData(USART,0x7E);
  wait_usart_tx;
  USART_SendData(USART,(char)frame.frame_size);
  wait_usart_tx;
  DMA_Cmd(USART_TX_DMA_CHANNEL,ENABLE);
  USART_DMACmd(USART,USART_TX_DMA_CHANNEL,ENABLE);
  task . callback_task_func(task . callback_task_data,
                            task . callback_task_data_size);
  return 0;
}

// temp
void loopwait(void* d,int s) {
  int x = 0;
  while(usart_dma_tx_wait)
    x = DMA_GetCurrDataCounter(DMA1_Channel7);
  usart_dma_tx_wait=1;
}

int recv_frame_no_wait() {
  if(usart_dma_rx_wait == 0) {// &&  usart_rx_buf[usart_rx_buf[1]+1] == 0xEF)
    usart_dma_rx_wait = 1;
    return  0;
  }
  else
    return -1;
}
