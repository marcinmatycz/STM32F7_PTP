/*****************************************************************************
 * @file top.c
 *
 * @brief Source file containing the setup function and loop function for top
 * layer of the program.
 *
 * @author Marcin Matycz
 * @date 14 mar 2020
 * @version v1.0
 *
 * @copyright 2020 Marcin Matycz - all rights reserved.
 ****************************************************************************/

/*****************************************************************************
                          		   INCLUDES
 *****************************************************************************/
#include "top.h"
#include "main.h"
#include "gpio.h"
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

void TOP_Setup(void)
{

}


void TOP_Loop(void)
{
	while(1)
	{
		  HAL_GPIO_WritePin(GPIOJ, LED_RED_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOJ, LED_GREEN_Pin, GPIO_PIN_SET);
		  HAL_Delay(1000);
		  HAL_GPIO_WritePin(GPIOJ, LED_RED_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOJ, LED_GREEN_Pin, GPIO_PIN_RESET);
		  HAL_Delay(1000);
	}
}

/*****************************************************************************
                       PRIVATE FUNCTION IMPLEMENTATION
 *****************************************************************************/
