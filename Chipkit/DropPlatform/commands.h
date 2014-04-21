/**
 * @file commands.h
 *
 * This file contains various definitions and function prototypes necessary for the command subsystem.
 *
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdint.h>		// Needed for the integer type definitions
#include <string.h>		// Needed for the string parsing functions
#include <stdlib.h>		// Needed for the atoi function
#include <stdbool.h>	// Needed for the boolean type

/// Define the maximum number of characters a command may have (including start and stop characters) to be 64
#define	COMMAND_MAX_LENGTH	30

/// Define the start character for the commands to be a # sign
#define COMMAND_START_CHAR	35

/// Define the stop character for the commands to be a $ sign
#define COMMAND_STOP_CHAR	36

/// Define the delimiter for the command arguments to be a comma
#define COMMAND_DELIM		44

/// The number of total commands this robot can respond to is 6
#define COMMAND_NUMBER_OF	2


/**
 * This data structure will contain the ASCII command code and a pointer to the function to handle the command
 */
typedef struct commandDef_t {
	char *commandType;			// ASCII characters for the command type string
	void (*function)(char*);	// Pointer to the function which will handle the command processing
} commandDef_t;


/// The master array containing all the commands defined for this robot.
/// This array is populated with the commands inside the function commands_init().
extern commandDef_t commandList[COMMAND_NUMBER_OF];


/**
 * Initialize the commands this robot supports by populating the commandsList[] array.
 */
void commands_init();


/**
 * This function will take the string received over the serial port and then parse it to see what command it is.
 *
 * This system will parse the command packets as long as they start with a #, end with a $, and have comma-delimited fields.
 * Then, it will use the first field to determine the type of command received. Once it determined the command received,
 * it will search the commandList[] array for the corresponding command. If it finds it, the function given in the array's
 * structure will be called to actually execute the command.

 * Should the packet not have any commas, the command interpreter will ignore the packet.
 *
 * @param receivedString A character array containing the characters received over the port since the last call of this function
 */
void commands_interpret(char* receivedString);


/************************************************************************/
/* The following functions are used by the command handling system      */
/*    DO NOT CALL THESE FUNCTIONS DIRECTLY!!!                           */
/************************************************************************/


/**
 * This function is called by the command handling system when the drop plane command is received
 *
 * Please pass a dummy argument into this command because the command system has not been tested with
 * arguementless commands.
 *
 * DO NOT CALL THIS FUNCTION DIRECTLY!!!
 *
 * @param args A character array of the arguments received for this command
 */
void comm_drop(char* args);

void comm_telem(char* args);

#endif
