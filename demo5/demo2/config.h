// config file for the demo2
#ifndef _CONFIG_H_
#define _CONFIG_H_


// for led1
#define _LED_1_GPIO_SET_  (GPIOB)
#define _LED_1_GPIO_PORT_ (0)
#define _LED_1_GPIO_CLK_  (3)
#if LED_1_GPIO_PORT > 7
#define _LED_1_GPIO_CR_ CRH
#define _LED_1_GPIO_CRI_ ((_LED_1_GPIO_PORT_ - 8) * 4)
#else
#define _LED_1_GPIO_CR_ CRL
#define _LED_1_GPIO_CRI_ ((_LED_1_GPIO_PORT_)     * 4)
#endif
// for led2
#define _LED_2_GPIO_SET_  (GPIOF)
#define _LED_2_GPIO_PORT_ (7)
#define _LED_2_GPIO_CLK_  (7)
#if LED_2_GPIO_PORT > 7
#define _LED_2_GPIO_CR_ CRH
#define _LED_2_GPIO_CRI_ ((_LED_2_GPIO_PORT_ - 8) * 4)
#else
#define _LED_2_GPIO_CR_ CRL
#define _LED_2_GPIO_CRI_ ((_LED_2_GPIO_PORT_)     * 4)
#endif
#endif // _CONFIG_H_
