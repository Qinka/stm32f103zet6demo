// config file for the demo4
#ifndef _CONFIG_H_
#define _CONFIG_H_


// for beep
// for led
#define _LED_GPIO_SET_  (GPIOB)
#define _LED_GPIO_PORT_ (5)
#define _LED_GPIO_CLK_  (3)
#if 5 > 7
#define _LED_GPIO_CR_ CRH
#define _LED_GPIO_CR_MOV_ ((5 - 8) * 4)
#else
#define _LED_GPIO_CR_ CRL
#define _LED_GPIO_CR_MOV_ ((5)     * 4)
#endif

// for pwm
#define _PWM_CHx_       2
#define _PWM_CCR_       CCR2
#define _PWM_ARR_       899
#define _PWM_LVL_MAX_   300
#define _PWM_REMAP_     2

#if 3 == 2
#define _PWM_REMAP_MOV_ (8)
#define _PWM_TIMx_      (TIM2)
#define _PWM_CLK_       (0)
#endif // TIM2

#if 3 == 3
#define _PWM_REMAP_MOV_ (10)
#define _PWM_TIMx_      (TIM3)
#define _PWM_CLK_       (1)
#endif // TIM3


#if 3 == 4
#define _PWM_REMAP_MOV_ (12)
#define _PWM_TIMx_      (TIM4)
#define _PWM_CLK_       (2)
#endif // TIM4


#if 2 == 1
#define _PWM_CCMR_ CCMR1
#define _PWM_CCMR_MOV_ 3
#endif // CH1

#if 2 == 2
#define _PWM_CCMR_ CCMR1
#define _PWM_CCMR_MOV_ 11
#endif // CH2

#if 2 == 3
#define _PWM_CCMR_ CCMR2
#define _PWM_CCMR_MOV_ 3
#endif // CH3

#if 2 == 4
#define _PWM_CCMR_ CCMR1
#define _PWM_CCMR_MOV_ 11
#endif // CH4

#endif // _CONFIG_H_
