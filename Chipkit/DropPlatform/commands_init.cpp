/**
 * @file commands_init.c
 *
 * This file contains the commands_init() function, which is a part of the command subsystem
 *
 */
#include "commands.h"

commandDef_t commandList[COMMAND_NUMBER_OF];

/**
 * Initialize the commands this payload supports by populating the commandsList[] array.
 */
void commands_init() {

	/**
	 * Add the drop plane command to the payload
	 * The command string is #forward,$
	 * The function called upon reception of the command is comm_drop().
	 */
	commandList[0].commandType = "drop";
	commandList[0].function = &(comm_drop);

	commandList[1].commandType = "telem";
	commandList[1].function = &(comm_telem);
}
