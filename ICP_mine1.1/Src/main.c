/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "rw_app.h"
#include "AD7606.h"

//#include "lua.h"
//#include "lauxlib.h"
//#include "lualib.h"
#include "lua_test.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void fat_error_test(uint8_t ret);

int platform_init(void)
{
  rw_DriverParams_t     params;
  int                   ret =0;
  char                  libVersion[20]="";
  char                  module_mac[6] ="";
	
  //host_platformInit();	
  DPRINTF("Host platform init...success\r\n");
  
  //rak module driver init
  wifi_init_params(&params);
  ret =rw_sysDriverInit(&params);
  if(ret != RW_OK)
  {
    DPRINTF("RAK module platform init...failed code=%d\r\n", ret);
    while(1){printf("ret=%d ",ret);HAL_Delay(1000);/*HAL_GPIO_WritePin(BT_EN_GPIO_Port,BT_EN_Pin,GPIO_PIN_SET);*/}; 
  }
  rw_getLibVersion(libVersion); 
  DPRINTF("rak wifi LibVersion:%s\r\n", libVersion);
  rw_getMacAddr(module_mac);
  DPRINTF("rak wifi module-MAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n", module_mac[0],module_mac[1],module_mac[2],module_mac[3],module_mac[4],module_mac[5]);
  
  return RW_OK;
}

//  @ fuction:  
//  @ description:  
//  @ input:
//  @ output:
//  @ note: 测试电池电压
void BAT_ADC_test(void){
	  uint16_t AD_value = 0;
	  float AD_transfer = 0;
	
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,50);//等待转换完成
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_EOC_REG)){
		AD_value = HAL_ADC_GetValue(&hadc1);
	  AD_transfer = AD_value*2*3.3/0xfff;//由于分压，需要乘上2
			printf("%d ADC=%fv ",AD_value,AD_transfer);
		}
		if(AD_transfer < 3.2f){
			HAL_GPIO_WritePin(BT_EN_GPIO_Port,BT_EN_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_RESET);
		 while(1){
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
		  HAL_Delay(5000);
			 printf("%d ADC=%fv ",AD_value,AD_transfer);
		 }
		}
}

void fat_test(void){
  FATFS fs;                 // Work area (file system object) for logical drive
	FIL fil;                  // file objects
	 
	uint32_t byteswritten;                /* File write counts */
	uint32_t bytesread;                   /* File read counts */
	uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
	uint8_t rtext[100];                     /* File read buffers */
	char filename[] = "STM32cube.txt";
//	char filename[] = "S.txt";
	
    printf("\r\n ****** FatFs Example ******\r\n\r\n");
 
    /*##-1- Register the file system object to the FatFs module ##############*/
    retSD = f_mount(&fs, "", 0);
    if(retSD)
    {
        printf(" mount error : %d \r\n",retSD);
        Error_Handler();
    }
    else
        printf(" mount sucess!!! \r\n");
     
    /*##-2- Create and Open new text file objects with write access ######*/
    retSD = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if(retSD)
        //printf(" open file error : %d\r\n",retSD);
		fat_error_test(retSD);
    else
        printf(" open file sucess!!! \r\n");
     
    /*##-3- Write data to the text files ###############################*/
    retSD = f_write(&fil, wtext, sizeof(wtext), (void *)&byteswritten);
    if(retSD)
        //printf(" write file error : %d\r\n",retSD);
		fat_error_test(retSD);
    else
    {
        printf(" write file sucess!!! \r\n");
        printf(" write Data : %s\r\n",wtext);
    }
     
    /*##-4- Close the open text files ################################*/
    retSD = f_close(&fil);
    if(retSD)
        //printf(" close error : %d\r\n",retSD);
		fat_error_test(retSD);
    else
        printf(" close sucess!!! \r\n");
     
    /*##-5- Open the text files object with read access ##############*/
    retSD = f_open(&fil, filename, FA_READ);
    if(retSD)
        //printf(" open file error : %d\r\n",retSD);
		fat_error_test(retSD);
    else
        printf(" open file sucess!!! \r\n");
     
    /*##-6- Read data from the text files ##########################*/
    retSD = f_read(&fil, rtext, sizeof(rtext), (UINT*)&bytesread);
    if(retSD)
        //printf(" read error!!! %d\r\n",retSD);
		fat_error_test(retSD);
    else
    {
        printf(" read sucess!!! \r\n");
        printf(" read Data : %s\r\n",rtext);
    }
     
    /*##-7- Close the open text files ############################*/
    retSD = f_close(&fil);
    if(retSD)  
        //printf(" close error!!! %d\r\n",retSD);
		fat_error_test(retSD);
    else
        printf(" close sucess!!! \r\n");
     
    /*##-8- Compare read data with the expected data ############*/
    if(bytesread == byteswritten)
    { 
        printf(" FatFs is working well!!!\r\n");
    }
}

void fat_error_test(uint8_t ret){
  switch(ret){
		case FR_OK :
			printf("磁盘正常\r\n");
			break;
		case FR_INVALID_DRIVE:
			printf("驱动器无效\r\n");
			break;
		case FR_NOT_READY:
			printf("磁盘驱动器无法工作\r\n");
			break;
		case FR_DISK_ERR:
			printf("底层磁盘IO错误\r\n");
			break;
		case FR_NO_PATH:
			printf("找不到路径\r\n");
			break;
		case FR_INVALID_NAME:
			printf("文件名无效\r\n");
			break;
		case FR_NO_FILE:
			printf("找不到该文件\r\n");
			break;
		case FR_EXIST:
			printf("文件已存在\r\n");
			break;
		default:
			printf("其他错误%d\r\n",ret);
			break;
	}
		
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
   int  		ret = 0;
	//uint16_t AD_value = 0;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART6_UART_Init();
  MX_ADC1_Init();
  MX_SPI3_Init();
  MX_SDIO_SD_Init();
  MX_TIM2_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */
//	HAL_Delay(5000);//延长5s用于连接蓝牙
//	BAT_ADC_test();
	
   HAL_GPIO_WritePin(BT_EN_GPIO_Port,BT_EN_Pin,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(ICP_EN_GPIO_Port,ICP_EN_Pin,GPIO_PIN_SET);
	 AD_CONVEST_PWM_Init(10);//10kHz
	 
	 //uint8_t ss[] = "sdf";
	 
	 HAL_Delay(5000);//延长5s用于连接蓝牙
	 BAT_ADC_test();//这样不打开电池开关，无法继续
	 platform_init();
	 rw_appdemo_context_init();
	 
	 //rw_network_startSTA();
	 rw_network_startAP();//start AP
	 HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	 AD7606_Init();
	 
	 //lua_main_test();//测试lua脚本

	 //fat_test();//测试fat
	 do_file_script("autorun.lua");//测试lua SD卡读入
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
		//HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
		//printf("ok ");
//		HAL_UART_Transmit(&huart6,ss,sizeof(*ss),3);
//		DPRINTF("ddd ");
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1,50);//等待转换完成
//		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_EOC_REG)){
//		AD_value = HAL_ADC_GetValue(&hadc1);
//			printf("%d ADC=%fv ",AD_value,AD_value*3.3/0xfff);
//		}
		HAL_Delay(1000);
		
		if((ret =rw_sysDriverLoop()) != RW_OK){
       DPRINTF("rw_sysDriverLoop error =%d\r\n", ret);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
    }
        
    if (app_demo_ctx.rw_connect_status == STATUS_OK && app_demo_ctx.rw_ipquery_status == STATUS_OK) {
			RAK_TcpServer_EventHandle();
     //RAK_TcpClient_EventHandle();
    //  RAK_UdpServer_EventHandle();
    //  RAK_UdpClient_EventHandle();
    }
		else if (app_demo_ctx.rw_connect_status == STATUS_FAIL || app_demo_ctx.rw_ipquery_status == STATUS_FAIL) {
      DPRINTF("reconnect and ipquery...\r\n");
      rw_appdemo_context_init(); 
      rw_sysDriverReset();
      rw_network_init(&conn, DHCP_CLIENT, NULL);
    }

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
		  HAL_Delay(10000);
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
	while(1){
		printf("Wrong parameters value: file %s on line %d\r\n", file, line);
		HAL_Delay(1000);
	}
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
