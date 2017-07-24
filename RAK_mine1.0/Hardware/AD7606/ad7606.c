#include "ad7606.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "main.h"
#include "stm32f4xx.h"

/*
PC5 -O- OS 0							
PB0 -O- OS 1
PB1 -O- OS 2
RANGE		0:+/-5V 
PA3 -O- CONVST A&B					rising edge:start to convert
PC4 -O- _CS
PA4 -I- BUSY
PA2 -O- ICP_EN
*/

#define OS2(State)	HAL_GPIO_WritePin(AD_OS2_GPIO_Port,AD_OS2_Pin,(GPIO_PinState)State)
#define OS1(State)	HAL_GPIO_WritePin(AD_OS1_GPIO_Port,AD_OS1_Pin,(GPIO_PinState)State)
#define OS0(State)	HAL_GPIO_WritePin(AD_OS0_GPIO_Port,AD_OS0_Pin,(GPIO_PinState)State)
#define SPI_CS(State)	HAL_GPIO_WritePin(AD_SPI1_CS_GPIO_Port,AD_SPI1_CS_Pin,(GPIO_PinState)State)
#define SPI_SCK(State)	HAL_GPIO_WritePin(AD_SPI1_SCK_GPIO_Port,AD_SPI1_SCK_Pin,(GPIO_PinState)State)
#define SPI_MISO()	HAL_GPIO_ReadPin(AD_SPI1_MISO_GPIO_Port,(GPIO_PinState)AD_SPI1_MISO_Pin)

union _AD7606_BUF AD7606_BUF;

void AD7606_Init(void)
{
	AD7606_SetOsRate(AD_OS_NO);
}

//  @ fuction:  
//  @ description:  过采样位解码
//  @ input:
//  @ output:
//  @ note: 
void AD7606_SetOsRate(AD7606_OS_Rate rate)
{
	switch(rate)
	{
		case AD_OS_NO:{OS2(0);OS1(0);OS0(0);break;}
		case AD_OS_X2:{OS2(0);OS1(0);OS0(1);break;}
		case AD_OS_X4:{OS2(0);OS1(1);OS0(0);break;}
		case AD_OS_X8:{OS2(0);OS1(1);OS0(1);break;}
		case AD_OS_X16:{OS2(1);OS1(0);OS0(0);break;}
		case AD_OS_X32:{OS2(1);OS1(0);OS0(1);break;}
		case AD_OS_X64:{OS2(1);OS1(1);OS0(0);break;}
		default:{OS2(0);OS1(0);OS0(0);break;}//111 invalid
	}
}

void AD7606_Read4CH(void)
{
	uint8_t i,j;
	uint8_t value;
	for(i=0;i<8;i++)
	{
		SPI_CS(0);//chip select
		for(j=0;j<8;j++)
		{
			value <<=1;
			if((AD_SPI1_MISO_GPIO_Port->IDR & AD_SPI1_MISO_Pin) != 0)
				value |= 1;
			AD_SPI1_SCK_GPIO_Port->BSRR = AD_SPI1_SCK_Pin;//set high
			SPI_SCK(0);//set low
		}
		SPI_CS(1);//chip deselect
		AD7606_BUF.bytebuf[i] = value;
	}
}

//  @ fuction:  
//  @ description:  AD_CONVEST_PWM
//  @ input:
//  @ output:
//  @ note: TIM2 channal 4 --> output: f_khz Hz
void AD_CONVEST_PWM_Init(uint8_t f_khz){
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 70-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1200/f_khz-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 600/f_khz;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);
}

//  @ fuction:  
//  @ description:  AD7606_EXTI_Handle
//  @ input:
//  @ output:
//  @ note: 
void AD7606_handle(void){
	uint8_t i = 0;
	
  AD7606_Read4CH();
	
//	for(i=0;i<4;i++) 
//	{
//		printf("AD7606 = %d ",AD7606_BUF.shortbuf[0+i]);//会造成阻塞，Wifi无法初始化
//		
//	}
	//i = AD7606_BUF.bytebuf[0];
}


