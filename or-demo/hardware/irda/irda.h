#ifndef _IRDA_H_
#define	_IRDA_H_

void IRDA_init(void);

#define IRDA_ID 0

//红外遥控使用的GPIO及时钟
#define IRDA_GPIO_PORT            GPIOA
#define IRDA_GPIO_CLK             RCC_APB2Periph_GPIOA
#define IRDA_TIM_CLK              RCC_APB1Periph_TIM2
#define IRDA_TIM                  TIM2
#define IRDA_Channel              TIM_Channel_4
#define IRDA_CCx                  TIM_IT_CC4
#define IRDA_GetCapture(x)        TIM_GetCapture4(x)
#define IRDA_OC_PolarityConfig(x,y) TIM_OC4PolarityConfig(x,y)
#define IRDA_GPIO_PIN             GPIO_Pin_3
#define IRDA_GPIO_PORT_SOURCE     GPIO_PortSourceGPIOD
#define IRDA_GPIO_PIN_SOURCE      GPIO_PinSource7

//中断相关
#define IRDA_IRQn                TIM2_IRQn
#define IRDA_IRQHandler          TIM2_IRQHandler
#define IRDA_EXTI_LINE           EXTI_Line7
#define IRDA_EXTI_IRQN           EXTI9_5_IRQn
#define IRDA_EXTI_IRQHANDLER_FUN EXTI9_5_IRQHandler

//读取引脚的电平
#define  IrDa_DATA_IN()	   GPIO_ReadInputDataBit(IRDA_GPIO_PORT,IRDA_GPIO_PIN)

#endif // _IRDA_H_
