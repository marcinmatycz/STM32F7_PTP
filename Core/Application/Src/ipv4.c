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


typedef struct
{
	uint8_t* version_IHL;				// 1 byte
	uint8_t* DSCP_ECN;					// 1 byte
	uint8_t* total_length;				// 2 bytes
	uint8_t* identification;			// 2 bytes
	uint8_t* flags_fragment_offset;		// 2 bytes
	uint8_t* time_to_live;				// 1 byte
	uint8_t* protocol;					// 1 byte
	uint8_t* header_checksum;			// 2 bytes
	uint8_t* source_address;			// 4 bytes
	uint8_t* destination_address;		// 4 bytes
	uint8_t* options;					// ? bytes skip for now TODO
	uint8_t* data;						// rest

} IPFrame;


/*****************************************************************************
                        PRIVATE FUNCTION DECLARATION
 *****************************************************************************/
void PrintIPHeader(IPFrame *ipframe);

/*****************************************************************************
	                       INTERFACE IMPLEMENTATION
 *****************************************************************************/

void IP_ReadIPHeader(Frame *frame)
{
	IPFrame ipframe = {0};

	ipframe.version_IHL = &(frame->data[0]);
	ipframe.DSCP_ECN = &(frame->data[1]);
	ipframe.total_length = &(frame->data[2]);
	ipframe.identification = &(frame->data[4]);
	ipframe.flags_fragment_offset = &(frame->data[6]);
	ipframe.time_to_live = &(frame->data[8]);
	ipframe.protocol = &(frame->data[9]);
	ipframe.header_checksum = &(frame->data[10]);
	ipframe.source_address = &(frame->data[12]);
	ipframe.destination_address = &(frame->data[16]);
	ipframe.data = &(frame->data[20]);

	PrintIPHeader(&ipframe);
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

