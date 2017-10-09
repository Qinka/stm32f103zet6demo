#ifndef _IRDA_H_
#define	_IRDA_H_

void IRDA_init(void);

#define IRDA_ID 0

//红外遥控使用的GPIO及时钟
#define IRDA_GPIO_PORT            GPIOB
#define IRDA_GPIO_CLK             RCC_APB2Periph_GPIOB
#define IRDA_TIM_CLK              RCC_APB1Periph_TIM4
#define IRDA_TIM                  TIM4
#define IRDA_Channel              TIM_Channel_4
#define IRDA_CCx                  TIM_IT_CC4
#define IRDA_GetCapture(x)        TIM_GetCapture4(x)
#define IRDA_OC_PolarityConfig(x,y) TIM_OC4PolarityConfig(x,y)
#define IRDA_GPIO_PIN             GPIO_Pin_9

//中断相关
#define IRDA_IRQn                TIM4_IRQn
#define IRDA_IRQHandler          TIM4_IRQHandler

//读取引脚的电平
#define  IrDa_DATA_IN()	   GPIO_ReadInputDataBit(IRDA_GPIO_PORT,IRDA_GPIO_PIN)

#endif // _IRDA_H_
