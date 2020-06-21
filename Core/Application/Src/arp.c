/*****************************************************************************
 * @file arp.c
 *
 * @brief TODO brief description of arp.c file
 *
 * @author Marcin Matycz
 * @date 12 cze 2020
 * @version v1.0
 *
 * @copyright 2020 Marcin Matycz - all rights reserved.
 ****************************************************************************/

/*****************************************************************************
                          		   INCLUDES
 *****************************************************************************/
#include "arp.h"

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
extern UART_HandleTypeDef huart1;

void ARP_Respond(Frame *frame)
{
	Frame frame_to_send = {0};
	uint8_t to_send[42];
	uint8_t mac_address[6];

	// Making an ethernet frame

	// Destination MAC address
	for (int i = 8; i < 8+6; i++)
		to_send[i-8] = *((frame->data) + i);

	frame_to_send.destination_address = &to_send[0];

	// Source MAC address
	ETH_GetMACAddress(&mac_address);
	for (int i = 6; i < 12; i++)
		to_send[i] = mac_address[i-6];

	frame_to_send.source_address = &to_send[6];

	// Ethertype
	to_send[12] = 0x08;
	to_send[13] = 0x06;

	frame_to_send.length_type = &to_send[12];

	// ARP Hardware type
	to_send[14] = 0x00;
	to_send[15] = 0x01;

	// ARP Protocol type
	to_send[16] = 0x08;
	to_send[17] = 0x00;

	// ARP Hardware address length and protocol address length
	to_send[18] = 6;
	to_send[19] = 4;

	// ARP operation - reply
	to_send[20] = 0x00;
	to_send[21] = 0x02;

	// ARP sender hardware address
	for (int i = 22; i < 28; i++)
		to_send[i] = mac_address[i-22];

	// ARP sender protocol address
	to_send[28] = 192;
	to_send[29] = 168;
	to_send[30] = 0;
	to_send[31] = 174;

	// ARP Target hardware address
	for (int i = 8; i < 8+6; i++)
		to_send[i+24] = *((frame->data) + i);

	// ARP Target protocol address
	for (int i = 14; i < 14+4; i++)
		to_send[i+24] = *((frame->data) + i);

	frame_to_send.data = &to_send[14];

	ETH_TransmitFrame(&frame_to_send, 42);
}

/*****************************************************************************
                       PRIVATE FUNCTION IMPLEMENTATION
 *****************************************************************************/
