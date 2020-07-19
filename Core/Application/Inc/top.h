/*****************************************************************************
 * @file top.h
 *
 * @brief Header file containing the setup function and loop function for top
 * layer of the program.
 *
 * @author Marcin Matycz
 * @date 14 mar 2020
 * @version v1.0
 *
 * @copyright 2020 Marcin Matycz - all rights reserved.
 ****************************************************************************/
#pragma once

/*****************************************************************************
                          		   INCLUDES
 *****************************************************************************/

/*****************************************************************************
                            PUBLIC DEFINES / MACROS
 *****************************************************************************/

/** @brief 	System initialization function
 */
void TOP_Setup(void);

/** @brief 	System loop function
 */
void TOP_Loop(void);

void TOP_SetTransmitFlag(void);



/*****************************************************************************
                      PUBLIC STRUCTS / ENUMS / VARIABLES
 *****************************************************************************/

/*****************************************************************************
                        PUBLIC INTERFACE DECLARATION
 *****************************************************************************/
