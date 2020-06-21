/*****************************************************************************
 * @file ipv4.c
 *
 * @brief TODO brief description of ipv4.c file
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
#include "ipv4.h"
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
void PrintIPHeader(IPFrame *ipframe);

/*****************************************************************************
	                       INTERFACE IMPLEMENTATION
 *****************************************************************************/

void IP_ReadIPHeader(Frame *frame, IPFrame *ipframe)
{

	ipframe->version_IHL = &(frame->data[0]);
	ipframe->DSCP_ECN = &(frame->data[1]);
	ipframe->total_length = &(frame->data[2]);
	ipframe->identification = &(frame->data[4]);
	ipframe->flags_fragment_offset = &(frame->data[6]);
	ipframe->time_to_live = &(frame->data[8]);
	ipframe->protocol = &(frame->data[9]);
	ipframe->header_checksum = &(frame->data[10]);
	ipframe->source_address = &(frame->data[12]);
	ipframe->destination_address = &(frame->data[16]);
	ipframe->data = &(frame->data[20]);

	PrintIPHeader(ipframe);

}


void IP_SendEchoReply(Frame *frame, IPFrame *ipframe)
{

	//TODO

	Frame frame_to_send = {0};
	uint8_t to_send[1500];
	uint8_t mac_address[6];

	// Destination MAC address
	for (int i = 0; i < 6; i++)
		to_send[i] = *((frame->source_address) + i);

	frame_to_send.destination_address = &to_send[0];

	// Source MAC address
	ETH_GetMACAddress(&mac_address);
	for (int i = 6; i < 12; i++)
		to_send[i] = mac_address[i-6];

	frame_to_send.source_address = &to_send[6];

	// Ethertype
	to_send[12] = 0x08;
	to_send[13] = 0x00;

	frame_to_send.length_type = &to_send[12];

	frame_to_send.data = &to_send[14];

	// Version / IHL
	to_send[14] = 0b01000101;

	// DSCP ECN
	to_send[15] = 0;

	// total length
	to_send[16] = ipframe->total_length[0];
	to_send[17] = ipframe->total_length[1];

	// identification
	to_send[18] = 0;
	to_send[19] = 0;

	// flags / fragment offset
	to_send[20] = 0;
	to_send[21] = 0;

	// TTL
	to_send[22] = 128;

	// protocol
	to_send[23] = 1;

	// Header checksum
	to_send[24] = 0;
	to_send[25] = 0;

	// TODO change for not fixed IP
	// source address
	to_send[26] = 192;
	to_send[27] = 168;
	to_send[28] = 0;
	to_send[29] = 174;

	// destination address
	to_send[30] = 192;
	to_send[31] = 168;
	to_send[32] = 0;
	to_send[33] = 173;


	// calculate header checksum
	uint32_t sum = 0;
	uint16_t header_checksum;
	for(int i = 0; i <20 ; i=i+2)
	{
		sum += ((uint16_t)(to_send[14+i]) << 8) | (to_send[14+i+1]);
	}

	header_checksum = sum + (sum>>16);
	header_checksum = ~header_checksum;
	//to_send[24] = header_checksum >> 8;
	//to_send[25] = header_checksum;


	// ICMP type
	to_send[34] = 0;

	// ICMP code
	to_send[35] = 0;

	// ICMP checksum
	to_send[36] = 0;
	to_send[37] = 0;

	// ICMP identifier
	to_send[38] = ipframe->data[4];
	to_send[39] = ipframe->data[5];

	// ICMP sequence number
	to_send[40] = ipframe->data[6];
	to_send[41] = ipframe->data[7];

	uint16_t totallength = (ipframe->total_length[1]) | ((ipframe->total_length[0]) << 8);

	for (int i = 0; i < totallength - 20 - 8; i++)
	{
		to_send[i+42] = ipframe->data[8+i];
	}

	sum = 0;
	for(int i = 0; i <totallength - 20 ; i = i+2)
	{
		sum += ((uint16_t)(to_send[34+i]) << 8) | (to_send[34+i+1]);
	}

	header_checksum = sum + (sum>>16);
	header_checksum = ~header_checksum;
	//to_send[36] = header_checksum >> 8;
	//to_send[37] = header_checksum;



	ETH_TransmitFrame(&frame_to_send, totallength+14);

}

/*****************************************************************************
                       PRIVATE FUNCTION IMPLEMENTATION
 *****************************************************************************/
extern UART_HandleTypeDef huart1;

void PrintIPHeader(IPFrame *ipframe)
{
	char s[12];


	HAL_UART_Transmit(&huart1, "IP destination address: ", strlen("IP destination address: "), 1000);
	for (int i = 0; i < 4; i++)
	{
		snprintf(s, 4, "%d", *((ipframe->destination_address) + i));
		HAL_UART_Transmit(&huart1, s, strlen(s), 1000);
		HAL_UART_Transmit(&huart1, ".", 1, 1000);
	}


	HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);


	HAL_UART_Transmit(&huart1, "IP source address: ", strlen("IP source address: "), 1000);

	for (int i = 0; i < 4; i++)
	{
		snprintf(s, 4, "%d", *((ipframe->source_address) + i));
		HAL_UART_Transmit(&huart1, s, strlen(s), 1000);
		HAL_UART_Transmit(&huart1, ".", 1, 1000);
	}
	HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);


	HAL_UART_Transmit(&huart1, "Protocol: ", strlen("Protocol: "), 1000);
	snprintf(s, 4, "%d", *(ipframe->protocol));
	HAL_UART_Transmit(&huart1, s, strlen(s), 1000);
	HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);



//	HAL_UART_Transmit(huart, "Length/type: ", strlen("Length/type: "), 1000);
//	for (int i = 0; i < 2; i++)
//	{
//		snprintf(s, 3, "%X", *((frame->length_type) + i));
//		HAL_UART_Transmit(huart, s, 2, 1000);
//		HAL_UART_Transmit(huart, " ", 1, 1000);
//	}
//	HAL_UART_Transmit(huart, "\r\n", 2, 1000);
//
//	HAL_UART_Transmit(huart, "Data: ", strlen("Data: "), 1000);
//	HAL_UART_Transmit(huart, frame->data, 1500, 1000);
//	HAL_UART_Transmit(huart, "\r\n", 2, 1000);

}

