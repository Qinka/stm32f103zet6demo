/**
 * OLED driver
 * header
 */

#ifndef _OLED_H_
#define _OLED_H_


// peripheral define
#define XLevelL			0x00
#define XLevelH			0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row			64
#define	Brightness	0xCF 
#define X_WIDTH 		128
#define Y_WIDTH 		64

// define the ports for OLED
//! the stm32f103zet6 firmware library required

#define OLED_PORT         GPIOA
// OLED module's CS port
#define OLED_CS_PORT      GPIOA
#define OLED_CS_PIN       GPIO_Pin_5
#define OLED_CS_PORT_CLK  RCC_APB2Periph_GPIOA
#define OLED_CS_Clr()     GPIO_ResetBits(OLED_CS_PORT,OLED_CS_PIN)
#define OLED_CS_Set()     GPIO_SetBits(OLED_CS_PORT,OLED_CS_PIN)
// OLED module's D/C port
#define OLED_DC_PORT      GPIOA
#define OLED_DC_PIN       GPIO_Pin_4
#define OLED_DC_PORT_CLK  RCC_APB2Periph_GPIOA
#define OLED_DC_Clr()     GPIO_ResetBits(OLED_DC_PORT,OLED_DC_PIN)
#define OLED_DC_Set()     GPIO_SetBits(OLED_DC_PORT,OLED_DC_PIN)
// OLED module's MOSI port (SPI) <=> SDA
#define OLED_SDA_PORT     GPIOA
#define OLED_SDA_PIN      GPIO_Pin_6
#define OLED_SDA_PORT_CLK RCC_APB2Periph_GPIOA
#define OLED_SDA_Clr()    GPIO_ResetBits(OLED_SDA_PORT,OLED_SDA_PIN)
#define OLED_SDA_Set()    GPIO_SetBits(OLED_SDA_PORT,OLED_SDA_PIN)
// OLED module's CLK  port (SPI) <=> SCL
#define OLED_SCL_PORT     GPIOA
#define OLED_SCL_PIN      GPIO_Pin_7
#define OLED_SCL_PORT_CLK RCC_APB2Periph_GPIOA
#define OLED_SCL_Clr()    GPIO_ResetBits(OLED_SCL_PORT,OLED_SCL_PIN)
#define OLED_SCL_Set()    GPIO_SetBits(OLED_SCL_PORT,OLED_SCL_PIN)

// OLED control code
// write command
#define OLED_CMD  0
// write data
#define OLED_DATA 1

// OLED control
void OLED_WrDat(unsigned char dat); // write data
void OLED_WrCmd(unsigned char cmd); // write command
void OLED_SetPos(unsigned char x, unsigned char y); // set up beginning-position 
void OLED_Fill(unsigned char bmp_dat); // fill all
void OLED_CLS(void); // clean up
void OLED_Init(void); // initialization
void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[]); // 6 x 8 str
void OLED_x16Str(unsigned char x, unsigned char y, unsigned char ch[]); // 8 x 16 str
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N); // 16 x 16 str
void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1,
             unsigned char y1, unsigned char BMP[]); // display bmp
void OLED_6x8Hex(unsigned char x, unsigned char y, unsigned char d);
#endif // _OLED_H_
