/**
  ******************************************************************************
  * File Name          : SPI.c
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
/*
    PB3     ------> SPI1_SCK    ---->  NC
    PB5     ------> SPI1_MOSI   ---->  WS2812B
*/
const unsigned short LightNumberMax = 100, LightNumber = 100;
unsigned char Ws2812bData[900] = {0};
unsigned char Ws2812bResetData[100] = {0};
/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

/* SPI1 init function */
void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PB3     ------> SPI1_SCK
    PB5     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_SPI1_ENABLE();

    /* SPI1 DMA Init */
    /* SPI1_TX Init */
    hdma_spi1_tx.Instance = DMA1_Channel3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi1_tx);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration    
    PB3     ------> SPI1_SCK
    PB5     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_5);

    /* SPI1 DMA DeInit */
    HAL_DMA_DeInit(spiHandle->hdmatx);
  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/**
  * @brief  初始化Ws2812b数组数据为0码
  * @retval None
  */
void Ws2812bInit(void)
{
	unsigned char i, j;
	for(i = 0; i < LightNumberMax; i++)		//LightNumberMax 个灯
	{
		for(j = 0; j < 3; j++)							//3 个颜色
		{		
			Ws2812bData[i * 9 + j * 3 + 1] = 0x92;	//全为0的 0码
			Ws2812bData[i * 9 + j * 3 + 2] = 0x49;
			Ws2812bData[i * 9 + j * 3 + 3] = 0x24;
		}
	}
}

/**
  * @brief  初始化Ws2812b数组数据为0码
  * @retval 0码：100； 1码：110；只需改变中间那个1就可以
  */
void Ws2812bSet(unsigned char Number, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	unsigned char color, bit;
	unsigned char Color[3] = {Green, Red, Blue};
	Number -= 1;		//由从1开始计数 -> 从0开始计数
	
	for(color = 0; color < 3; color++)			//三个颜色
	{
		bit = 1;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x40;
			Color[color] <<= 1;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x08;
			Color[color] <<= 1;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x01;
			Color[color] <<= 1;
		
		bit = 2;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x20;
			Color[color] <<= 1;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x04;
			Color[color] <<= 1;
		
		bit = 3;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x80;
			Color[color] <<= 1;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x10;
			Color[color] <<= 1;
		if(Color[color] & 0x80)
			Ws2812bData[Number * 9 + color * 3 + bit] |= 0x20;		
	}
}

/**
  * @brief  刷新Ws2812b灯带
  * @retval None
  */
void Ws2812bRefresh(void)
{
	HAL_SPI_Transmit(&hspi1, Ws2812bResetData, 100, 100);						//537us低电平 复位
	HAL_SPI_Transmit_DMA(&hspi1, Ws2812bData, LightNumber * 9);		//灯带数据
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
