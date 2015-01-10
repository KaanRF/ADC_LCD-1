/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "4_bit_lcd.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/

#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)

 uint16_t ADC3ConvertedValue=0;   // unsigned olarak degiskenlerimizi belirledik
 uint32_t ADC3ConvertedVoltage=0;

 //GPIO_InitTypeDef GPIO_InitStructure;

 //int ticks1;

void deger_goster(uint32_t);
void ADC3_CH12_DMA_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  STM_EVAL_LEDInit(LED4);

  STM_EVAL_LEDOn(LED4);

  if (SysTick_Config(SystemCoreClock / 1000))
  {
    while (1);
  }


     initDiscovery();

   lcd_init();
   DelayMS(20);

    LCD_DATA(0x01,0);
    DelayMS(20);
  LCD_LINE(1);
  LCD_STR((const char*)"ADC-1 LCD ");
  DelayMS(200);
  LCD_LINE(2);
  LCD_STR((const char*)"Kaan KANDEMÝR");

  DelayMS(200);
    LCD_DATA(0x01,0);

      ADC3_CH12_DMA_Config();
      ADC_SoftwareStartConv(ADC3);

DelayMS(200);
            LCD_DATA(0x01,0);

  while (1)
  {			// 3.3volt benim usb de 2.951 volt o yüzden aþaðýda bölmede bu deðeri kullandým.
          ADC3ConvertedVoltage = ADC3ConvertedValue * 2951 / 0xFFF;   // 0xFFF = 4095 = 12bit

          deger_goster(ADC3ConvertedVoltage);

        STM_EVAL_LEDToggle(LED4);

        DelayMS(250);
  }
}


void  ADC3_CH12_DMA_Config(void)
{
     ADC_InitTypeDef       ADC_InitStructure;
      ADC_CommonInitTypeDef ADC_CommonInitStructure;
      DMA_InitTypeDef       DMA_InitStructure;
      GPIO_InitTypeDef      GPIO_InitStructure;

    //SIMDI SIRAYLA ADC DMA GPIO BUS YOLLARININ CLOCKLARINI AKTIF EDELIM

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
       RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

    //DMA AYARLARINI YAPALIM
       DMA_InitStructure.DMA_Channel = DMA_Channel_2;
         DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
         DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue;
         DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
         DMA_InitStructure.DMA_BufferSize = 1;
         DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
         DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
         DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
         DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
         DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
         DMA_InitStructure.DMA_Priority = DMA_Priority_High;
         DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
         DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
         DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
         DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
         DMA_Init(DMA2_Stream0, &DMA_InitStructure);
         DMA_Cmd(DMA2_Stream0, ENABLE);
    //GPIO AYARLARI YAPILIYOR C PORTUNDA ISLEM YAPICAZ
          GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
          GPIO_Init(GPIOC, &GPIO_InitStructure);

    //ADC MODULU AYARLARI

          ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
          ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
          ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
          ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
          ADC_CommonInit(&ADC_CommonInitStructure);

    //ADC3 AYARLARI

          ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
          ADC_InitStructure.ADC_ScanConvMode = DISABLE;
          ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
          ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
          ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
          ADC_InitStructure.ADC_NbrOfConversion = 1;
          ADC_Init(ADC3, &ADC_InitStructure);
    //ADC3 REGULE KANAL 12
          ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);
          ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

    //ADC ICIN DMA AKTIF
          ADC_DMACmd(ADC3, ENABLE);
    //ADC3 AKTIF
          ADC_Cmd(ADC3, ENABLE);


}

void deger_goster(uint32_t ham)
{
    uint32_t v=0,mv=0;
    char text[30];

    v=(ADC3ConvertedVoltage)/1000;
    mv = (ADC3ConvertedVoltage%1000);
    DelayMS(200);

    sprintf((char*)text,"ADC: %02ld,%03ld Volt",v,mv);
    DelayMS(200);
  LCD_LINE(1);
  DelayMS(200);
  LCD_STR2((u8*)text);

    sprintf((char*)text,"ADC: %04ld mv    ",ADC3ConvertedVoltage);
    DelayMS(200);
  LCD_LINE(2);
  DelayMS(200);
  LCD_STR2((u8*)text);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif



uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
    return 0; // ses çipini kullanýyorsanýz tek sample veriyi burada return ile döndürün.
}
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
    return; // ses çipini kullanýyorsanýz  burada çipe veri aktarýmý DMA sona ermiþ
}
