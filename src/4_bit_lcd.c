/*
 * 4_bit_lcd.c
 *
 *  Created on: Mar 1, 2013
 *      Author: Kaan
 */
#include "stm32f4xx.h"
#include "4_bit_lcd.h"


extern void delay(uint32_t ticks) //us sev�yes�nde beklme suresi
{
  while(ticks--);
}

void lcd_init() //lcd ayarla� set ed�liyor
{
    CMD_PORT &= ~(LCD_E);  //LCD_E = 0;                    //Enable 0 la
    CMD_PORT &= ~(LCD_RS); //LCD_RS = 0;                 //yazmaya haz�rlan

    DelayMS(30);                //BASLAMADAN �NCE STAB�L�ZASYON AYARI
    LCD_NYB(0x30,0);              //BASLAMA SATIRI
    DelayMS(5);
    LCD_NYB(0x30,0);              //BA�LAMA SATIRI
    DelayMS(1);
    LCD_DATA(0x02,0);           //4 B�T G�NDER�LECEK HAZIRLAN, 1 line and 5*7 font
    LCD_DATA(0x28,0);           //4 B�T G�NDER�LECEK HAZIRLAN, 2 line and 5*10 font
    LCD_DATA(0x0c,0);           //4 B�T G�NDER�LECEK HAZIRLAN, 2 line and 5*7 font
    LCD_DATA(0x01,0);           //EKRANI TEM�ZLE
    LCD_DATA(0x06,0);           //move cursor right after write
}

//--------------------------------------------------------------------------------//
void LCD_DATA(unsigned char data,unsigned char type){

    WaitLCDBusy();                  //MESGUL OLAB�L�R B�RAZ GEC�KME KOYDUK

    if(type == CMD){
        CMD_PORT &= ~(LCD_RS);                 //KOMUT MOD
    } else {
        CMD_PORT |= LCD_RS;                 //KARAKTER/DATA MOD
    }

    LCD_NYB(data>>4,type);               //WRITE THE UPPER NIBBLE
    LCD_NYB(data,type);                  //WRITE THE LOWER NIBBLE
}
//--------------------------------------------------------------------------------//
void WaitLCDBusy(void){
    DelayMS(2);              //DELAY 1 M�L�SAN�YE
}
//--------------------------------------------------------------------------------//
void LCD_NYB(unsigned char nyb,unsigned char type){
    DATA_PORT &= DATA_CLR;    //LCD_PORT &= 0xF0;                     //CLEAR LOWER PORT NIBBLE
    DATA_PORT |= (nyb<<DATA); //LCD_PORT |= (nyb & 0x0F);             //SATIR B�LG�S� G�NDER

    if(type == CMD){
        CMD_PORT &= ~(LCD_RS);                 //KOMUT MOD
    } else {
        CMD_PORT |= LCD_RS;                 //KARAKTER/DATA MOD
    }

    CMD_PORT |= LCD_E;    //LCD_E = 1;          //AKT�F LCD DATA LINE
    delay(100);
    CMD_PORT &= ~(LCD_E); //LCD_E = 0;       //DEAKT�F LCD DATA LINE
}
//--------------------------------------------------------------------------------//
void LCD_STR(const char *text){ // lcd 1. sat�r yazmak �c�n
    while(*text){
        LCD_DATA(*text++,1);
    }
}
void LCD_STR2(unsigned char *text){ //lcd 2. sat�r i�in yaz
    while(*text)
    {
        LCD_DATA(*text++,1);
    }
}
//--------------------------------------------------------------------------------//
void LCD_LINE(char line){ //lcd goto yer�ne line olarak belirledik
    switch(line){
        case 0:
        case 1:
            LCD_DATA(LINE1,0);
            break;
        case 2:
            LCD_DATA(LINE2,0);
            break;
    }
}
//--------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------//
void DelayMS(unsigned int ms){
    unsigned int x;
    for(x=0;x<ms;x++)
        delay(600);
}

void initDiscovery(void)
{
  RCC->CFGR = RCC_CFGR_SW_HSE;
  RCC->AHB1ENR = RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOCEN;
  RCC->CR |= RCC_CR_HSEON;

  while((RCC->CR & RCC_CR_HSERDY)==0);

  GPIOC->MODER=GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0|GPIO_MODER_MODER9_0;
  GPIOA->MODER=GPIO_MODER_MODER8_0|GPIO_MODER_MODER9_0;

  GPIOC->OSPEEDR = GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1 | GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR9_1;
  GPIOA->OSPEEDR = GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR9_1;
}
