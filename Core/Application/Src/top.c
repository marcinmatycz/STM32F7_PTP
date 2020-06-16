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
#include "eth.h"
#include "arp.h"
#include <string.h>
/*****************************************************************************
                          PRIVATE DEFINES / MACROS
 *****************************************************************************/

/*****************************************************************************
                      PRIVATE STRUCTS / ENUMS / VARIABLES
 *****************************************************************************/

/*****************************************************************************
                        PRIVATE FUNCTION DECLARATION
 *****************************************************************************/
void TOP_PollForEthernetFrame(void);
/*****************************************************************************
	                       INTERFACE IMPLEMENTATION
 *****************************************************************************/

void TOP_Setup(void)
{
	ETH_InitDescriptors();
	ETH_Start();
}


void TOP_Loop(void)
{
	while(1)
	{
		TOP_PollForEthernetFrame();

	}
}

/*****************************************************************************
                       PRIVATE FUNCTION IMPLEMENTATION
 *****************************************************************************/
extern UART_HandleTypeDef huart1;
void TOP_PollForEthernetFrame(void)
{
	char s[12];

	Frame E_frame = {0};
	if(ETH_ReceiveFrame(&E_frame))
	{
//		HAL_UART_Transmit(&huart1, "Ethertype: ", strlen("Ethertype: "), 1000);
//		for (int i = 0; i < 2; i++)
//		{
//			snprintf(s, 3, "%X", E_frame.length_type[i]);
//			HAL_UART_Transmit(&huart1, s, strlen(s), 1000);
//			HAL_UART_Transmit(&huart1, " ", 1, 1000);
//		}
//		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);

		if(E_frame.length_type[0] == 0x08 && E_frame.length_type[1] == 0x06 )
		{
			ARP_Respond(&E_frame);
			//ETH_TransmitFrame(&E_frame);

		}
	}
		//IP_ReadIPHeader(&E_frame);

}
