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
#include "ipv4.h"
#include "error_blinks.h"
#include <string.h>
#include <stdio.h>
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
	ERR_InitBlink();
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
int count = 0;
void TOP_PollForEthernetFrame(void)
{

	Frame E_frame = {0};
	IPFrame ip_frame = {0};

	char s[5] = {0};

	if(ETH_ReceiveFrame(&E_frame))
	{

		HAL_UART_Transmit(&huart1, "Frame received.\r\n", strlen("Frame received.\r\n"), 1000);


		// ARP request
		if(E_frame.length_type[0] == 0x08 && E_frame.length_type[1] == 0x06 )
		{
			HAL_UART_Transmit(&huart1, "ARP request received.\r\n", strlen("ARP request received.\r\n"), 1000);
			ARP_Respond(&E_frame);
		}

		// IP frame
		if(E_frame.length_type[0] == 0x08 && E_frame.length_type[1] == 0x00 )
		{
			HAL_UART_Transmit(&huart1, "IP frame received.\r\n", strlen("IP frame received.\r\n"), 1000);

			IP_ReadIPHeader(&E_frame, &ip_frame);
			// ICMP Request
			if(*(ip_frame.protocol) == 1)
			{
				HAL_UART_Transmit(&huart1, "Echo Request received.\r\n", strlen("Echo Request received.\r\n"), 1000);
				IP_SendEchoReply(&E_frame, &ip_frame);
				count++;
			}

		}



		snprintf(s, 4, "%d", count);
		HAL_UART_Transmit(&huart1, s, strlen(s), 1000);
		HAL_UART_Transmit(&huart1, "\r\n", strlen("\r\n"), 1000);

	}

}
