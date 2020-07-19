/*****************************************************************************
 * @file error_blinks.c
 *
 * @brief TODO brief description of error_blinks.c file
 *
 * @author Marcin Matycz
 * @date Jul 19, 2020
 * @version v1.0
 *
 * @copyright 2020 Marcin Matycz - all rights reserved.
 ****************************************************************************/

/*****************************************************************************
                          		   INCLUDES
 *****************************************************************************/
#include "error_blinks.h"
#include "main.h"
/*****************************************************************************
                          PRIVATE DEFINES / MACROS
 *****************************************************************************/

/*****************************************************************************
                      PRIVATE STRUCTS / ENUMS / VARIABLES
 *****************************************************************************/

/*****************************************************************************
                        PRIVATE FUNCTION DECLARATION
 *****************************************************************************/

/*****************************************************************************
	                       INTERFACE IMPLEMENTATION
 *****************************************************************************/
void ERR_InitBlink(void)
{
	for(int i = 0; i < 3; i++)
	{
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		HAL_Delay(200);
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		HAL_Delay(200);
	}
}

void ERR_LoopBlink(void)
{

}

void ERR_ConfBlink(void)
{
	for(int i = 0; i < 2; i++)
	{
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		HAL_Delay(50);
	}
}

void ERR_ErrBlink(void)
{
	for(int i = 0; i < 2; i++)
	{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(50);
	}
}


/*****************************************************************************
                       PRIVATE FUNCTION IMPLEMENTATION
 *****************************************************************************/
