#include "stm32f10x.h"


int _exit(void)
{
}

u8 t_counter = 0;

int main(void)
{
  //group
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  // init usart1
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
  USART_DeInit(USART1);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure2;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure2);
  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  TIM_ICInitTypeDef TIM5_ICInitStructure;
  TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
  TIM_Cmd(TIM5,ENABLE);
  // send
  USART_SendData(USART1, 'D');
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
  USART_SendData(USART1, '\r');
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
  USART_SendData(USART1, '\n');
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
  while(1);
}

void TIM5_IRQHandler(void)
{
  u16 tsr = TIM5 -> SR;
  if(t_counter >= 0xFF)
    goto reset;
  if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET) {
    ++ t_counter;
    goto next;
  }
  if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) {
    if(t_counter == 0) {
      ++ t_counter;
      TIM_SetCounter(TIM5,0);
      TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);
      goto next;
    }
    else
      goto reset;
  }
  return;
 reset:
  USART_SendData(USART1, t_counter);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
  USART_SendData(USART1, '\r');
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
  USART_SendData(USART1, '\n');
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
  TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
  t_counter = 0;
 next:
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update);
}

