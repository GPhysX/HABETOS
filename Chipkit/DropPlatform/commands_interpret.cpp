/**
 * @file commands_interpret.c
 *
 * This file contains the function comands_interpret(), which is a part of the command subsystem.
 *
 */
#include "commands.h"

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
void commands_interpret(char* receivedString) {
	/*
	 * These variables are for the buffering of the command string section
	 */

	// This buffer will hold the command until it is fully received
	static char commandBuffer[COMMAND_MAX_LENGTH];

	// The location of the last character in the command buffer
	static uint8_t idx_commandBuf = 0;

	// Have we previously found the start character?
	static bool isStartPresent = false;

	// Determine the length of the received string
	uint8_t len_recStr = strlen(receivedString);

	char *ptr_startChar = 0;	// The memory address of the start character in the string
	char *ptr_stopChar = 0;		// The memory address of the stop character in the string
	uint8_t pos_startChar = 0;	// The position of the start character in the string
	uint8_t pos_stopChar = 0;	// The position of the stop character in the string

	/*
	 * This section contains the variables necessary for the detection of the command type
	 */
	char *ptr_firstComma;						// The memory address of the first comma of the command string
	char commandString[15];						// The decoded command type
	char commandArguments[COMMAND_MAX_LENGTH];	// The arguments for the received command

	uint8_t pos_firstComma = 0;					// The position in the received buffer of the first comma
	uint8_t i;


	/*
	 * This section is responsible for buffering the command strings
	 */

	// Check to see if we have previously found the start character
	if (!isStartPresent) {
		// If we haven't found the start character locate it, and if it isn't present then do not continue
		if ( !(ptr_startChar = strchr(receivedString, COMMAND_START_CHAR))) {
			return;
		} else {
			// We have found the start character
			isStartPresent = true;
			idx_commandBuf = 0;		// Restart the buffer counter at the beginning
		}
	}


	// Since the start character is present, see if the stop character is present
	ptr_stopChar = strchr(receivedString, COMMAND_STOP_CHAR);

	// Calculate the relative position of the first character of the command sequence only if the start character is in the string
	if (ptr_startChar)
		pos_startChar = ptr_startChar - receivedString + 1;

	// Calculate the relative position of the last character of the command sequence only if the stop character is in the string
	if (ptr_stopChar)
		pos_stopChar = ptr_stopChar - receivedString;


	// Determine what characters in the received buffer should be copied into the command storage buffer
	if ( (ptr_startChar == 0) && (ptr_stopChar != 0) ) {
		// If there is no start character in the received string, but there is a stop character then copy starting at the beginning and stop at the stop character

		// Copy the desired characters
		strncpy( &(commandBuffer[idx_commandBuf]), &(receivedString[0]), pos_stopChar );
		idx_commandBuf += pos_stopChar;

	} else if ( (ptr_startChar != 0) && (ptr_stopChar == 0) ) {
		// If there is a start character in the received string, but there is no stop character then copy starting at the start character and stop at the end of the received string

		// Copy the desired characters
		strncpy( &(commandBuffer[0]), ptr_startChar+1, (len_recStr-pos_startChar) );
		idx_commandBuf += (len_recStr-pos_startChar);

		// Exit the function to wait for more of the command to be received
		return;

	} else if ( (ptr_startChar != 0) && (ptr_stopChar != 0) ) {
		// If there is a start and a stop character in the received string, then copy the characters between them

		// Copy the desired characters
		strncpy( &(commandBuffer[0]), ptr_startChar+1, (pos_stopChar-pos_startChar) );
		idx_commandBuf += (pos_stopChar-pos_startChar);

	} else {
		// If there is no start or stop character present in the received string, then copy the entire string

		// Copy the desired characters
		strncpy( &(commandBuffer[idx_commandBuf]), &(receivedString[0]), len_recStr );
		idx_commandBuf += len_recStr;

		// Exit the function to wait for more of the command to be received
		return;
	}

	// Null terminate the command buffer
	commandBuffer[idx_commandBuf+1] = '\0';

	// Clear out the start character present variable since we have finished the command
	isStartPresent = false;


	/*
	 * This section is responsible for the detection of the command type and the proper calling of the command's function
	 */

	// Locate the first comma in the command
	if (!(ptr_firstComma = strchr(commandBuffer, COMMAND_DELIM)) ) {
		// No comma exists, so return to avoid a null pointer
		return;
	}

	// Calculate the relative position of the comma in the string
	pos_firstComma = ptr_firstComma - commandBuffer;

	// Copy the characters between the beginning of the command and the first comma into the buffer
	strncpy(commandString, commandBuffer, pos_firstComma);

	// Copy the arguments for the command into their own buffer for storage
	strncpy(commandArguments, (ptr_firstComma+1), (idx_commandBuf-pos_firstComma));

	// Null terminate the strings
	commandString[pos_firstComma] = '\0';
	commandArguments[idx_commandBuf-pos_firstComma] = '\0';


	/*
	 * These print statements allow for debugging of the received string
	 */
//	usart0_transmit_char_len(commandString, pos_firstComma);
//	usart0_transmit_char("\n\r");
//	usart0_transmit_char_len(commandArguments, (idx_commandBuf-pos_firstComma));
//	usart0_transmit_char("\n\r");

	// Iterate through an array of the commands to find the one that was just received
	for (i=0; i<COMMAND_NUMBER_OF; i++) {
		if ( strcmp(commandString, commandList[i].commandType) == 0) {
			// If the command type matches one stored, then call the function for the command and pass the rest of the received string as its arguments
			(*(commandList[i].function))(commandArguments);

			// The command was found, no need to keep searching
			break;
		}
	}

	// Zero out the arrays to remove the old arguments and command information
	for (i=0; i<COMMAND_MAX_LENGTH; i++) {
		commandArguments[i] = 0;
		commandBuffer[i] = 0;
	}
	for (i=0; i<15; i++) {
		commandString[i] = 0;
	}
}

