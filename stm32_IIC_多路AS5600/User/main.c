
#include "main.h"
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./delay/core_delay.h"

#include "string.h"

#include "soft_IIC.h"

uint8_t aRxBuffer;
extern int IIC_index;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
 void deal_with_pluse(void);
 void input_io_init(void);
//int main(void) 
//{
//	SystemClock_Config();
//	MX_USARTx_Init();

//	HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
//	dwt_init();
//	float angle_val[IIC_NUM];
//	AS5600_Init();
//	while(1)
//	{   	
//		
//		IIC_index++;
//		if(IIC_index>IIC_NUM-1)
//		{
//			IIC_index=0;
//		}
//		angle_val[IIC_index]=Get_Angle();
//		
//		printf("ID %d angle_val = %.3f\r\n",IIC_index,angle_val[IIC_index]);
//		
//		delay_ms(50);	

//	}
//} 


/************************************************************
Function: void ZeroPositionProgram(void)
Description: MT6701Zero Program
Author: Joe
Version:V0.1
Input: none
Output: none
Return: none
History: 
1.Joe 2020.5.27 build this moudle
2.<author> <time> <version > <desc>
***********************************************************/	
 uint8_t data_32 = 0x00;
 uint8_t data_33 = 0x00;
 uint8_t data_03 = 0x00;
 uint8_t data_04 = 0x00;	
 uint8_t temp = 0x00;
	
 void ZeroPositionProgram(void)
 {   
	 //readout zeroposition register
     data_32=MT681X_Read(0x32);
	 data_33=MT681X_Read(0x33);    
	 //clear zeroposition register but high 4 bits of 0x32
	 MT681X_Write(0x32,0x00|(data_32&0xF0));
	 MT681X_Write(0x33,0x00);
	 
	 //readout angle register
     data_03=MT681X_Read(0x03);
	 data_04=MT681X_Read(0x04);  
	 
	 //wirte current angle to zero position register
	 temp = (data_03>>4)|(data_32&0xF0);
	 MT681X_Write(0x32,temp);
	 temp = ((data_03&0x0F)<<4)|(data_04>>4);
	 MT681X_Write(0x33,temp);	
	 //write to eeprom
	 MT681X_Write(0x09,0xB3);
	 MT681X_Write(0x0A,0x05);
	 
 }
uint16_t datatemp[5]={5,6,7,8,9};
uint16_t Angle = 0;

int main(void) 
{
	SystemClock_Config();
	MX_USARTx_Init();

	HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
	dwt_init();
	float angle_val[IIC_NUM];
	MT681X_Init();
	while(1)
	{   	
		
		IIC_index++;
		if(IIC_index>IIC_NUM-1)
		{
			IIC_index=0;
		}
		
		datatemp[0]=MT681X_Read(0x03);
		datatemp[1]=MT681X_Read(0x04);
		Angle = (((datatemp[0]&0x00ff)<<8)|(datatemp[1]&0x00fc))>>2;;
		angle_val[IIC_index]= Angle;
		
//		angle_val[IIC_index]=Get_Angle_MT();
		
		printf("ID %d angle_val = %.3f\r\n",IIC_index,angle_val[IIC_index]);
		
		delay_ms(50);	

	}
} 



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  HAL_UART_Transmit(&husartx,&aRxBuffer,1,0);
  HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
 void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) {};
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while(1) {};
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
