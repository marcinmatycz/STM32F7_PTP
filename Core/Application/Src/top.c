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
bool transmit_flag = 0;


/*****************************************************************************
                        PRIVATE FUNCTION DECLARATION
 *****************************************************************************/
void TOP_PollForEthernetFrame(void);
void SendTestFrame(void);

/*****************************************************************************
	                       INTERFACE IMPLEMENTATION
 *****************************************************************************/

void TOP_Setup(void)
{
	ETH_InitDescriptors();
	ETH_Start();
	ERR_InitBlink();
}

extern UART_HandleTypeDef huart1;
void TOP_Loop(void)
{
	while(1)
	{
		TOP_PollForEthernetFrame();

		if(1 == transmit_flag)
		{
			// transmit frame
			SendTestFrame();
			HAL_UART_Transmit(&huart1, "Test frame transmitted.\r\n", strlen("Test frame transmitted.\r\n"), 1000);
			transmit_flag = 0;
		}

	}
}

void TOP_SetTransmitFlag(void)
{
	transmit_flag = 1;
}

/*****************************************************************************
                       PRIVATE FUNCTION IMPLEMENTATION
 *****************************************************************************/

int count = 0;
void TOP_PollForEthernetFrame(void)
{

	Frame E_frame = {0};
	IPFrame ip_frame = {0};

	char s[5] = {0};

	if(ETH_ReceiveFrame(&E_frame))
	{

		HAL_UART_Transmit(&huart1, "Frame received.\r\n", strlen("Frame received.\r\n"), 1000);




		// PTP frame
		if(E_frame.length_type[0] == 0x88 && E_frame.length_type[1] == 0xF7 )
		{
			HAL_UART_Transmit(&huart1, "PTP frame received.\r\n", strlen("PTP frame received.\r\n"), 1000);

		}









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




void SendTestFrame(void)
{
	Frame frame_to_send = {0};
	uint8_t to_send[21];
	uint8_t mac_address[6];

	// Making an ethernet frame

	// Destination MAC address (broadcast for testing TODO)
	for (int i = 8; i < 8+6; i++)
		to_send[i-8] = 0xFF;

	frame_to_send.destination_address = &to_send[0];

	// Source MAC address
	ETH_GetMACAddress(&mac_address);
	for (int i = 6; i < 12; i++)
		to_send[i] = mac_address[i-6];

	// FIXME artificial MACADDR for test
	to_send[6] = 71;

	frame_to_send.source_address = &to_send[6];

	// Ethertype of IEEE 1588 Precision Time Protocol over IEEE 802.3 Ethernet
	to_send[12] = 0x88;
	to_send[13] = 0xF7;

	frame_to_send.length_type = &to_send[12];

	// ARP Hardware type
	to_send[14] = 'E';
	to_send[15] = 'T';
	to_send[16] = 'H';

	to_send[17] = 't';
	to_send[18] = 'e';
	to_send[19] = 's';
	to_send[20] = 't';

	frame_to_send.data = &to_send[14];

	ETH_TransmitFrame(&frame_to_send, 21);
}
