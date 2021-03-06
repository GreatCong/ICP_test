/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define GSPI3_PWD_Pin GPIO_PIN_3
#define GSPI3_PWD_GPIO_Port GPIOC
#define BAT_ADC1_Pin GPIO_PIN_0
#define BAT_ADC1_GPIO_Port GPIOA
#define BT_EN_Pin GPIO_PIN_1
#define BT_EN_GPIO_Port GPIOA
#define ICP_EN_Pin GPIO_PIN_2
#define ICP_EN_GPIO_Port GPIOA
#define AD_CONVEST_Pin GPIO_PIN_3
#define AD_CONVEST_GPIO_Port GPIOA
#define AD_BUSY_Pin GPIO_PIN_4
#define AD_BUSY_GPIO_Port GPIOA
#define AD_BUSY_EXTI_IRQn EXTI4_IRQn
#define AD_SPI1_SCK_Pin GPIO_PIN_5
#define AD_SPI1_SCK_GPIO_Port GPIOA
#define AD_SPI1_MISO_Pin GPIO_PIN_6
#define AD_SPI1_MISO_GPIO_Port GPIOA
#define AD_SPI1_MOSI_Pin GPIO_PIN_7
#define AD_SPI1_MOSI_GPIO_Port GPIOA
#define AD_SPI1_CS_Pin GPIO_PIN_4
#define AD_SPI1_CS_GPIO_Port GPIOC
#define AD_OS0_Pin GPIO_PIN_5
#define AD_OS0_GPIO_Port GPIOC
#define AD_OS1_Pin GPIO_PIN_0
#define AD_OS1_GPIO_Port GPIOB
#define AD_OS2_Pin GPIO_PIN_1
#define AD_OS2_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_8
#define LED_RED_GPIO_Port GPIOA
#define GSPI3_SCK_Pin GPIO_PIN_3
#define GSPI3_SCK_GPIO_Port GPIOB
#define GSPI3_MISO_Pin GPIO_PIN_4
#define GSPI3_MISO_GPIO_Port GPIOB
#define GSP3_MOSI_Pin GPIO_PIN_5
#define GSP3_MOSI_GPIO_Port GPIOB
#define GSPI3_INT_Pin GPIO_PIN_6
#define GSPI3_INT_GPIO_Port GPIOB
#define GSPI3_INT_EXTI_IRQn EXTI9_5_IRQn
#define GSPI3_CS_Pin GPIO_PIN_7
#define GSPI3_CS_GPIO_Port GPIOB
#define WIFI_EN_Pin GPIO_PIN_9
#define WIFI_EN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define WIFI_SPI                             SPI3
#define AD_SPI                               SPI1//可选择是硬件SPI还是模拟SPI
/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
