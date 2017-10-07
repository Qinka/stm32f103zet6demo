/**
 *
 */


#include "irda.h"
#include "stm32f10x.h"
#include "misc.h"



// for IR receive
u8  RmtSta=0;
u16 Dval;  //下降沿时计数器的值
u32 RmtRec=0; //红外接收到的数据
u8  RmtCnt=0; //按键按下的次数 


void IRDA_init(void) {
  GPIO_InitTypeDef        GPIO_InitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef       TIM_ICInitStructure;
  RCC_APB2PeriphClockCmd(IRDA_GPIO_CLK,ENABLE); //使能 PORTA 时钟
  RCC_APB1PeriphClockCmd( IRDA_TIM_CLK,ENABLE); //TIM4 时钟使能
  GPIO_InitStructure.GPIO_Pin   = IRDA_GPIO_PIN;        //PA3 输入
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;       //上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IRDA_GPIO_PORT, &GPIO_InitStructure);   //初始化GPIOB.9
  GPIO_SetBits(IRDA_GPIO_PORT,IRDA_GPIO_PIN);     //GPIOB.9输出高
  TIM_TimeBaseStructure.TIM_Period = 9999; //设定计数器自动重装值 最大10ms溢出 (9999 + 1)
  TIM_TimeBaseStructure.TIM_Prescaler = 71; //预分频器,1M的计数频率,1us (71 + 1)
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //TIM向上计数模式
  TIM_TimeBaseInit(IRDA_TIM, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
  TIM_ICInitStructure.TIM_Channel = IRDA_Channel;  // 选择输入 Ix4映射到TIx上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 8个定时器时钟周期滤波
  TIM_ICInit(IRDA_TIM, &TIM_ICInitStructure);//初始化定时器输入捕获通道
  TIM_Cmd(IRDA_TIM,ENABLE );                           //使能定时器4
  NVIC_InitStructure.NVIC_IRQChannel = IRDA_IRQn;        //TIM3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      //IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化外设NVIC寄存器
  TIM_ITConfig(IRDA_TIM,TIM_IT_Update|IRDA_CCx,ENABLE);//允许更新中断 , 允许 CC4IE 捕获
}

void IRDA_IRQHandler(void) {
  if(TIM_GetITStatus(IRDA_TIM,TIM_IT_Update)!=RESET) {
    if(RmtSta&0x80) { //上次有数据被接收到了
      RmtSta&=~0X10;      //取消上升沿已经被捕获标记
      if((RmtSta&0X0F)==0X00)
        RmtSta|=1<<6; //标记已经完成一次键值信息采集
      if((RmtSta&0X0F)<14)
        RmtSta++;
      else {
        RmtSta&=~(1<<7);//清空引导标识
        RmtSta&=0XF0; //清空计数器
      }
    }
  }
  if(TIM_GetITStatus(IRDA_TIM,IRDA_CCx)!=RESET)  {
    if(GPIO_ReadInputDataBit(IRDA_GPIO_PORT,IRDA_GPIO_PIN)) { //上升沿捕获 
      TIM_OC4PolarityConfig(IRDA_TIM,TIM_ICPolarity_Falling); //下降沿捕获
      TIM_SetCounter(IRDA_TIM,0);                         //清空定时器值
      RmtSta|=0X10;                 //标记上升沿已经被捕获
    }
    else {  //下降沿捕获
      Dval=IRDA_GetCapture(IRDA_TIM);   //读取CCR1也可以清CC1IF标志位
      IRDA_OC_PolarityConfig(IRDA_TIM,TIM_ICPolarity_Rising);   //上升沿捕获
      if(RmtSta&0X10) { //完成一次高电平捕获
        if(RmtSta&0X80) { //接收到了引导码    
          if(Dval>300&&Dval<800) {  //560为标准值,560us
            RmtRec<<=1; //左移一位.
            RmtRec|=0; //接收到0
          }
          else if(Dval>1400&&Dval<1800) { //1680为标准值,1680us
            RmtRec<<=1; //左移一位.
            RmtRec|=1; //接收到1
          }
          else if(Dval>2200&&Dval<2600) {  //得到按键键值增加的信息  //2500为标准值2.5ms
            RmtCnt++;   //按键次数增加1次
            RmtSta&=0XF0; //清空计时器
          }
        }
        else if(Dval>4200&&Dval<4700) {    //4500为标准值4.5ms
          RmtSta|=1<<7; //标记成功接收到了引导码
          RmtCnt=0;  //清除按键次数计数器
        }
      }
      RmtSta&=~(1<<4);
    }
  }
  TIM_ClearITPendingBit(IRDA_TIM,TIM_IT_Update|IRDA_CCx);
}


/*
 * temp function
 */
u32 getCode(void) {
  while(RmtSta ^ (0b1 << 6));
  return RmtRec;
}
