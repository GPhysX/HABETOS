#ifndef ISU_BURNER_H_
#define ISU_BURNER_H

#include <stdint.h>

// The burner speed in hertz
#define	BURNER_CLK_SPEED		1000

// The burner period in microseconds
#define	BURNER_PERIOD			1000000/BURNER_CLK_SPEED

// Half the burner period in microseconds
#define	BURNER_HALF_PERIOD		BURNER_PERIOD/2

// Quarter of the burner period in microseconds
#define BURNER_QUARTER_PERIOD	BURNER_PERIOD/4

class ISU_Burner {

public:

	/**
	 * This is an enumerated list to contain all the command byte numbers the burner
	 * can handle.
	 */
	 enum commands {
		COMMAND_BURN = 1
	 };

	/**
	 * This is the constructor for a burner object
	 * 
	 * @param A_pin The ChipKit pin number for the A line of the burner comms interface
	 * @param B_pin The ChipKit pin number for the B line of the burner comms interface
	 */
	ISU_Burner(short A_pin, short B_pin);

	/**
	 * This function will send the burn command to the burner. It will heat
	 * the nichrome wire the the number of seconds specified in duration.
	 *
	 * @param duration The duration of the nichrome wire burn
	 */
	void sendBurn(short duration);

	/**
	 * This function will transmit the raw message specified in the buffer over the data link.
	 *
	 * @param messageBuf A character array containing the bytes to send over the data link
	 */
	void sendMessage(char *messageBuf, short length);


	void writeLogicLevel(uint8_t logicLevel);
private:
	short A_pinNum;
	short B_pinNum;

	/**
	 * This function will transmit the specified digital logic level (1 or 0)
	 *
	 * @param logicLevel The logic level to transmit
	 */
//	void writeLogicLevel(uint8_t logicLevel);
};

#endif
