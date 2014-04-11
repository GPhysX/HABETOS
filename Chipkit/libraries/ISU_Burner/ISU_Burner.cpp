#include "ISU_Burner.h"
#include <wiring.h>

ISU_Burner::ISU_Burner(short A_pin, short B_pin) {
	// Save the pin numbers that were passed in
	this->A_pinNum = A_pin;
	this->B_pinNum = B_pin;

	// Set the A and B pins to be open-collector output mode
	// The OPEN type is defined in MPIDE to be open collector & output
	// Also, the ChipKit will not disable the pull-ups during the mode change
	pinMode(A_pin, OPEN);
	pinMode(B_pin, OPEN);

	// Initialize the bus into a 0 state
	this->writeLogicLevel(LOW);
}


void ISU_Burner::sendBurn(short duration) {
	// A buffer for the burn message
	char burnMessage[2];

	// Populate the burn message buffer
//	burnMessage[0] = ISU_Burner::COMMAND_BURN;
	burnMessage[0] = 0xAA;
	burnMessage[1] = duration;

	// Send the burn command
	this->sendMessage(burnMessage, 2);
}


void ISU_Burner::sendMessage(char *messageBuf, short length) {
	// Send the initial start pulse (A greater than B) to signify the high time period of the clock
	this->writeLogicLevel(HIGH);
	delayMicroseconds(BURNER_HALF_PERIOD);
	this->writeLogicLevel(LOW);
	
	uint8_t logicLevelSend = 0;

	// Send the data out the bus, making sure that the data is high during the clock high period
	delayMicroseconds(BURNER_QUARTER_PERIOD);
	for (short i=0; i<(length*8); i++) {
		logicLevelSend = ( messageBuf[i/8] >> (i%8) ) & (0x01);
		delayMicroseconds(BURNER_HALF_PERIOD);
		this->writeLogicLevel(logicLevelSend);
		delayMicroseconds(BURNER_HALF_PERIOD);
	}
	// Place the bus back in a 0 logic state
	this->writeLogicLevel(0);
}

void ISU_Burner::writeLogicLevel(uint8_t logicLevel) {
	// The two pins should neve be the same logic level, it will cause an undetermined state
	digitalWrite(this->A_pinNum, logicLevel);
	digitalWrite(this->B_pinNum, (logicLevel ^ 0x01));
}
