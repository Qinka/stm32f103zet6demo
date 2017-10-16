/**
 * beep driver
 * header
 */

#ifndef _BEEP_H_
#define _BEEP_H_

#ifdef _BEEP_C_
// ports and pins
#define _BEEP_PORT_   GPIOC
#define _BEEP_PIN_    GPIO_Pin_0
#define _BEEP_CLK_    RCC_APB2Periph_GPIOC
#endif // _BEEP_C_

// function
void beep_init(void);
void beep_on(void);
void beep_off(void);
#endif // !_BEEP_H_
