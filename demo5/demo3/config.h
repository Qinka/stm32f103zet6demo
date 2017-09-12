// config file for the demo3
#ifndef _CONFIG_H_
#define _CONFIG_H_


// for beep
#define _BEEP_GPIO_SET_  (GPIOB)
#define _BEEP_GPIO_PORT_ (8)
#define _BEEP_GPIO_CLK_  (3)
#if _BEEP_GPIO_PORT_ > 7
#define _BEEP_GPIO_CR_ CRH
#define _BEEP_GPIO_CRI_ ((_BEEP_GPIO_PORT_ - 8) * 4)
#else
#define _BEEP_GPIO_CR_ CRL
#define _BEEP_GPIO_CRI_ ((_BEEP_GPIO_PORT_)     * 4)
#endif
#define _BEEP_BUF_SIZE_ (64)
#endif // _CONFIG_H_
