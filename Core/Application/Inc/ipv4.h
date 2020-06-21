/*****************************************************************************
 * @file ipv4.h
 *
 * @brief TODO brief description of ipv4.h file
 *
 * @author Marcin Matycz
 * @date 12 cze 2020
 * @version v1.0
 *
 * @copyright 2020 Marcin Matycz - all rights reserved.
 ****************************************************************************/
#pragma once

/*****************************************************************************
                          		   INCLUDES
 *****************************************************************************/
#include "eth.h"
/*****************************************************************************
                            PUBLIC DEFINES / MACROS
 *****************************************************************************/

/*****************************************************************************
                      PUBLIC STRUCTS / ENUMS / VARIABLES
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
	uint8_t* options;					// ? bytes - skip for now TODO
	uint8_t* data;						// rest

} IPFrame;


/*****************************************************************************
                        PUBLIC INTERFACE DECLARATION
 *****************************************************************************/

void IP_ReadIPHeader(Frame *frame, IPFrame *ipframe);
void IP_SendEchoReply(Frame *frame, IPFrame *ipframe);
