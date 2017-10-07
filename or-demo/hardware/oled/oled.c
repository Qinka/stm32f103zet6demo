/**
 * OLED driver
 * source
 */

#include "oled.h"
#include "../../delay.h"
#include "codetab.h"
#include "stm32f10x_conf.h"

/**
 * write data (8 bits)
 * @param dat the data to be write
 */
void OLED_WrDat(unsigned char dat) {
	unsigned char i;
	OLED_DC_Set();
	for(i=0;i<8;i++) {
		if((dat << i) & 0x80)
			OLED_SDA_Set();
		else
			OLED_SDA_Clr();
		OLED_SCL_Clr();
		OLED_SCL_Set();
	}
}

/**
 * write command (8 bits)
 * @param cmd command to be write
 */
void OLED_WrCmd(unsigned char cmd) {
	unsigned char i;
	OLED_DC_Clr();
	for(i=0;i<8;i++) {
		if((cmd << i) & 0x80)
			OLED_SDA_Set();
		else
      OLED_SDA_Clr();
		OLED_SCL_Clr();
		OLED_SCL_Set();
	}
}

/**
 *write position of cursor
 * @param x x position
 * @param y y position
 */
void OLED_SetPos(unsigned char x, unsigned char y) {
	OLED_WrCmd(0xb0 + y);
	OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
	OLED_WrCmd((x & 0x0f) | 0x01);
}

/**
 * fill all
 * @param mask (char)
 */
void OLED_Fill(unsigned char bmp_dat) {
	unsigned char y,x;
	for(y = 0; y < 8; y++) {
    OLED_WrCmd(0xb0 + y);
    OLED_WrCmd(0x01);
    OLED_WrCmd(0x10);
    for(x = 0;x < X_WIDTH; x++)
      OLED_WrDat(bmp_dat);
  }
}

/**
 * clean up
 */
void OLED_CLS(void) {
	OLED_Fill(0x00);
}

/**
 * initialization for OLED
 */
void OLED_Init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
  // enable the OLED's ports' clock
 	RCC_APB2PeriphClockCmd(OLED_CS_PORT_CLK | OLED_DC_PORT_CLK |
                         OLED_SDA_PORT_CLK | OLED_SCL_PORT_CLK,
                         ENABLE);
  // configure the pins
	GPIO_InitStructure.GPIO_Pin
    = OLED_CS_PIN | OLED_DC_PIN | OLED_SDA_PIN | OLED_SCL_PIN;
  // push-pull output
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(OLED_PORT, &GPIO_InitStructure);
  // set high level
 	GPIO_SetBits(OLED_PORT,OLED_CS_PIN | OLED_DC_PIN |
               OLED_SDA_PIN | OLED_SCL_PIN);

  /* initialize delay
     delayInit();
	*/

  delay_ms(500);
	OLED_CS_Clr();
	
	OLED_WrCmd(0xae);
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0,0xa1
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0,0xc8
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);
	OLED_SetPos(0,0);
}

/**
 * display the strings (6 x 8)
 * @param x x position
 * @param y y position
 * @param ch chars
 */
void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[]) {
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0') {
    c = ch[j]-32;
    if(x>126)
      x=0,y++;
    OLED_SetPos(x,y);
    for(i=0;i<6;i++)
      OLED_WrDat(F6x8[c][i]);
    x+=6;
    j++;
  }
}

/**
 * display the strings (8 x 16)
 * @param x x position
 * @param y y position
 * @param ch chars
 */
void OLED_8x16Str(unsigned char x, unsigned char y, unsigned char ch[]) {
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0') {
    c =ch[j]-32;
    if(x>120)
      x=0,y++;
    OLED_SetPos(x,y);
    for(i=0;i<8;i++)
      OLED_WrDat(F8X16[c*16+i]);
    OLED_SetPos(x,y+1);
    for(i=0;i<8;i++)
      OLED_WrDat(F8X16[c*16+i+8]);
    x+=8;
    j++;
  }
}

/**
 * display the Chinese (16 x 16)
 * @param x x position
 * @param y y position
 * @param N char
 */
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++) {
    OLED_WrDat(F16x16[adder]);
    adder += 1;
  }
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++) {
    OLED_WrDat(F16x16[adder]);
    adder += 1;
  }
}

/**
 * display bmp file
 * @param x0 x position
 * begin
 * @param y0 x position
 * end
 * @param x1 x position
 * @param y1 y position
 * @param BMP figure
 */
void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1,
              unsigned char y1, unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
    y=y1/8;
  else
		y=y1/8+1;
	for(y=y0;y<y1;y++) {
    OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
      OLED_WrDat(BMP[j++]);
  }
}

void OLED_6x8Hex(unsigned char x, unsigned char y, unsigned char d) {
  char str[3];
  str[1] = (d & 0x0F) + '0';
  str[0] = ((d >> 4) & 0x0F)  + '0';
  if (str[1] > '9')
    str[1] += 7;
  if (str[0] > '9')
    str[0] += 7;
  str[2] = '\0';
  OLED_6x8Str(x,y,str);
}
