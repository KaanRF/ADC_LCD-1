/*
 * 4_bit_lcd.h
 *
 *  Created on: Mar 27, 2013
 *      Author: Kaan
 */

 #define DATA_PORT GPIOC->ODR
#define CMD_PORT GPIOA->ODR

#define DATA 6
#define DATA_CLR 0xFFFFFC3F

#define LCD_E  0x100     //PA8
#define LCD_RS 0x200     //PA9

#define CMD 0
#define TXT 1

#define        LINE1    0x80        // ÝLK SATIR LCD_GOTO(1,X) ÝÇÝN
#define        LINE2    0xC0        // 2. SATIR YÖNLENDÝRME LCD_GOTO(2,X) ÝÇÝN

void WaitLCDBusy(void);
void lcd_init(void);
void LCD_DATA(unsigned char data,unsigned char type);
void LCD_NYB(unsigned char nyb,unsigned char type);
void LCD_STR(const char *text);
void LCD_STR2(unsigned char *text);
void LCD_LINE(char line);
void DelayMS(unsigned int ms);
void initDiscovery(void);

