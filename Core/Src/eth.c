/**
  ******************************************************************************
  * File Name          : ETH.c
  * Description        : This file provides code for the configuration
  *                      of the ETH instances.
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
#include "eth.h"

/* USER CODE BEGIN 0 */

ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB];
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB];

uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE];

/* USER CODE END 0 */

ETH_HandleTypeDef heth;

/* ETH init function */
void MX_ETH_Init(void)
{

  heth.Instance = ETH;
  heth.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;
  heth.Init.MACAddr[0] =   0x00;
  heth.Init.MACAddr[1] =   0x80;
  heth.Init.MACAddr[2] =   0xE1;
  heth.Init.MACAddr[3] =   0x00;
  heth.Init.MACAddr[4] =   0x00;
  heth.Init.MACAddr[5] =   0x00;
  heth.Init.RxMode = ETH_RXPOLLING_MODE;
  heth.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  heth.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

  /* USER CODE BEGIN MACADDRESS */
    
  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ETH_MspInit(ETH_HandleTypeDef* ethHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(ethHandle->Instance==ETH)
  {
  /* USER CODE BEGIN ETH_MspInit 0 */

  /* USER CODE END ETH_MspInit 0 */
    /* ETH clock enable */
    __HAL_RCC_ETH_CLK_ENABLE();
  
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ETH GPIO Configuration    
    PG14     ------> ETH_TXD1
    PG13     ------> ETH_TXD0
    PG11     ------> ETH_TX_EN
    PG8     ------> ETH_PPS_OUT
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PC4     ------> ETH_RXD0
    PA2     ------> ETH_MDIO
    PC5     ------> ETH_RXD1
    PA7     ------> ETH_CRS_DV 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_11|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN ETH_MspInit 1 */

  /* USER CODE END ETH_MspInit 1 */
  }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef* ethHandle)
{

  if(ethHandle->Instance==ETH)
  {
  /* USER CODE BEGIN ETH_MspDeInit 0 */

  /* USER CODE END ETH_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ETH_CLK_DISABLE();
  
    /**ETH GPIO Configuration    
    PG14     ------> ETH_TXD1
    PG13     ------> ETH_TXD0
    PG11     ------> ETH_TX_EN
    PG8     ------> ETH_PPS_OUT
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PC4     ------> ETH_RXD0
    PA2     ------> ETH_MDIO
    PC5     ------> ETH_RXD1
    PA7     ------> ETH_CRS_DV 
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_11|GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7);

  /* USER CODE BEGIN ETH_MspDeInit 1 */

  /* USER CODE END ETH_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

void ETH_InitDescriptors(void)
{
	HAL_ETH_DMATxDescListInit(&heth, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	HAL_ETH_DMARxDescListInit(&heth, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
}

void ETH_Start(void)
{
	HAL_ETH_Start(&heth);
}

bool ETH_ReceiveFrame(Frame *frame)
{

	if (HAL_ETH_GetReceivedFrame(&heth) == HAL_OK)
	{
		frame->destination_address = &Rx_Buff[0][0];
		frame->source_address = &Rx_Buff[0][6];
		//frame->tag = &Rx_Buff[0][12];
		frame->length_type = &Rx_Buff[0][12];
		frame->data = &Rx_Buff[0][14];

		//  TODO 			FROM LWIP, NEEDS VERIFICATION
		/* Release descriptors to DMA */
		/* Set Own bit in Rx descriptors: gives the buffers back to DMA */

		for (int i=0; i< heth.RxFrameInfos.SegCount; i++)
		{
			heth.RxFrameInfos.FSRxDesc->Status |= ETH_DMARXDESC_OWN;
			heth.RxFrameInfos.FSRxDesc = (ETH_DMADescTypeDef *)(heth.RxFrameInfos.FSRxDesc->Buffer2NextDescAddr);
		}

		/* Clear Segment_Count */
		heth.RxFrameInfos.SegCount =0;

		/* When Rx Buffer unavailable flag is set: clear it and resume reception */
		if ((heth.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)
		{
		/* Clear RBUS ETHERNET DMA flag */
			heth.Instance->DMASR = ETH_DMASR_RBUS;
		/* Resume DMA reception */
			heth.Instance->DMARPDR = 0;
		}

		return true;
	}
	return false;
}



HAL_StatusTypeDef ETH_TransmitFrame(Frame *frame, size_t length)
{



	ETH_PutInTxBuffer(frame->destination_address, 6, 0);
	ETH_PutInTxBuffer(frame->source_address, 6, 6);
	ETH_PutInTxBuffer(frame->length_type, 2, 12);
	ETH_PutInTxBuffer(frame->data, (length - 6 - 6 - 2), 14);


	while((heth.TxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET);

	HAL_ETH_TransmitFrame(&heth, length);
}


void ETH_PutInTxBuffer(uint8_t *data, size_t length, uint32_t offset)
{
	for(int i = offset; i < length + offset; i++)
		Tx_Buff[0][i] = data[i-offset];
}

void ETH_GetMACAddress(uint8_t *address)
{
	for (int i = 0; i < 6; i++)
		address[i] = heth.Init.MACAddr[i];
}



/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
