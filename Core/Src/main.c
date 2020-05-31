/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "eth.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "top.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB];
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB];
uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];
__IO uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_ETH_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  TOP_Setup();
  char* message = "This message was send from STM32F769 Discovery Board!\n";
  for(int i = 0; i < 6; i++)
	  Tx_Buff[0][i] = 0xFF;

  for(int i = 6; i < 12; i++)
  	  Tx_Buff[0][i] = 0xAA;

  Tx_Buff[0][12] = 0x05;
  Tx_Buff[0][13] = 0xDC;
  int j = 0;
  for(int i = 14; i < 14 + 55; i++)
  {
  	  Tx_Buff[0][i] = message[j];
  	  j++;
  }


  HAL_ETH_DMATxDescListInit(&heth, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
  HAL_ETH_DMARxDescListInit(&heth, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
  HAL_ETH_Start(&heth);

  HAL_UART_Transmit(&huart1, "Eluwina, świecie!\r\n", 20, 1000);
  HAL_UART_Transmit(&huart1, Tx_Buff[0], 1500, 1000);

  HAL_ETH_TransmitFrame(&heth, 1500);
  //TOP_Loop();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  char *s[3];
  while (1)
  {
    /* USER CODE END WHILE */
	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) //& ETH_MACDBGR_RFRCS)
	  //HAL_GPIO_WritePin(GPIOJ, LED_RED_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOJ, LED_GREEN_Pin, GPIO_PIN_SET);
	  if(heth.Instance->MACDBGR) //& ETH_MACDBGR_RFRCS)
		  HAL_GPIO_WritePin(GPIOJ, LED_RED_Pin, GPIO_PIN_SET);


	  if (HAL_ETH_GetReceivedFrame(&heth) == HAL_OK)
	  {
		  HAL_UART_Transmit(&huart1, "Destination address: ", 21, 1000);
		  for(int i = 0; i < 6; i++)
		  {
			snprintf(s, 5, "%d ", Rx_Buff[0][i]);
			HAL_UART_Transmit(&huart1, s, 5, 1000);
		  }
		  HAL_UART_Transmit(&huart1, "\n\r", 2, 1000);

		  HAL_UART_Transmit(&huart1, "Source address: ", 16, 1000);
		  for(int i = 6; i < 12; i++)
		  {
			snprintf(s, 5, "%d ", Rx_Buff[0][i]);
			HAL_UART_Transmit(&huart1, s, 5, 1000);
		  }
		  HAL_UART_Transmit(&huart1, "\n\r", 2, 1000);

		  HAL_UART_Transmit(&huart1, "Data length: ", 13, 1000);

		  snprintf(s, 7, "%d ", (uint16_t)(Rx_Buff[0][12] << 8) | (uint16_t)(Rx_Buff[0][13]));
		  HAL_UART_Transmit(&huart1, s, 7, 1000);

		  HAL_UART_Transmit(&huart1, "\n\r", 2, 1000);

		  HAL_UART_Transmit(&huart1, "Data: ", 6, 1000);

		  for(int i =14 ; i <ETH_RX_BUF_SIZE ; i++)
		  {
			  HAL_UART_Transmit(&huart1, &Rx_Buff[0][i], 1, 1000);
		  }
		  HAL_UART_Transmit(&huart1, "\n\r", 2, 1000);

		  while(1);
	  }


    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

