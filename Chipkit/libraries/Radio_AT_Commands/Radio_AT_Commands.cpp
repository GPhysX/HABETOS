#include "Radio_AT_Commands.h"

extern HardwareSerial Serial;

/**
 * The constructor for the AT command sender/parser
 *
 * @param radioSerialPort The object for the serial port where the radio is connected to
 */
Radio_AT_Command::Radio_AT_Command(HardwareSerial *radioSerialPort) {
	this->serialPort = radioSerialPort;
}


/**
 * Place the radio into AT command mode.
 *
 * @return 0 if entered command mode, 1 otherwise
 */
uint8_t Radio_AT_Command::startCommandMode() {
	uint16_t timeout = 500;

	// Make sure that the buffer is empty
	clearSerialBuffer();

	/*
	 * The way to enter the AT command mode is as follows:
	 * 1) Wait for 1 second
	 * 2) Send '+++' over the serial link
	 * 3) Wait for 1 second
	 */
	delay(1000);
	serialPort->write("+++");
	delay(1000);

	// We should receive a "CMD" prompt from the radio
	while( !serialPort->available() && timeout) {
		timeout--;
	}
	// Nothing was received from the radio, so error
	if ( !timeout ) {
		return(1);
	}

	// Make sure that the buffer is empty
	clearSerialBuffer();
	return(0);
}



/**
 * Exit the AT command mode
 */
void Radio_AT_Command::stopCommandMode() {
	// Make sure that the buffer is empty
	clearSerialBuffer();

	serialPort->write("ATO\r\n");
	delay(1000);

	// Make sure that the buffer is empty
	clearSerialBuffer();
}



/**
 * Reboot the radio
 */
void Radio_AT_Command::rebootRadio() {
	// Make sure that the buffer is empty
	clearSerialBuffer();

	serialPort->write("ATZ\r\n");

	// Make sure that the buffer is empty
	clearSerialBuffer();
}



/**
 * Save the current parameters to EEPROM
 */
void Radio_AT_Command::saveParametersToEEPROM() {
	// Make sure that the buffer is empty
	clearSerialBuffer();

	serialPort->write("AT&W\r\n");

	// Make sure that the buffer is empty
	clearSerialBuffer();
}


/**
 * Read an ATI value from the radio
 *
 * @param The ATI value to read from the radio
 * @param Pointer to a character array containing the output
 * @return The length of the returned output string, -1 if there is an error
 */
int16_t Radio_AT_Command::readATI(ATI_VALUES atiToRead, char *result) {
	char buffer[255] = {0};
	char *subString;
	char temp[10];
	uint8_t i=0;
	int32_t retval = -1;

	// Make sure that the buffer is empty
	clearSerialBuffer();
	
	// Formulate the command string
	strcat(buffer, "ATI");
	if (atiToRead != Radio_AT_Command::RADIO_VERSION) {
			strcat(buffer, itoa(atiToRead, temp, 10));
	}
	strcat(buffer, "?\r\n");
	
	// Send the command to the radio
	serialPort->write(buffer);

	// Wait for the response from the radio
	delay(1000);
	while ( serialPort->available() ) {
		buffer[i++] = serialPort->read();
		delay(10);
	}
	buffer[i] = 0;

	// Remove the command echo
	subString = strtok(buffer, "\r\n");

	// Pull out the result
	if (subString != NULL) {
		subString = strtok(NULL, "\r\n");
	}

	// Return the result 
	if (subString != NULL) {
		strcpy(result, subString);
		retval = strlen(result);
	}

	// Make sure that the buffer is empty
	clearSerialBuffer();

	return(retval);
}


/**
 * Read a parameter from the radio configuration
 *
 * @param The parameter to read
 * @return The value of the parameter, -1 if there is an error
 */
int32_t Radio_AT_Command::readParameter(ATS_REGISTERS reg) {
	char buffer[255] = {0};
	char *subString;
	char temp[10];
	uint8_t i=0;
	int32_t retval = -1;
	
	// Make sure that the buffer is empty
	clearSerialBuffer();

	// Formulate the command string
	strcat(buffer, "ATS");
	strcat(buffer, itoa(reg, temp, 10));
	strcat(buffer, "?\r\n");
	
	// Send the command to the radio
	serialPort->write(buffer);

	// Wait for the response from the radio
	delay(1000);
	while ( serialPort->available() ) {
		buffer[i++] = serialPort->read();
		delay(10);
	}
	buffer[i] = 0;

	// Remove the command echo
	subString = strtok(buffer, "\r\n");

	// Pull out the result
	if (subString != NULL) {
		subString = strtok(NULL, "\r\n");
	}

	// Return the result 
	if (subString != NULL) {
		retval = atoi(subString);
		
	}

	// Make sure that the buffer is empty
	clearSerialBuffer();

	return(retval);
}


/**
 * Set a parameter in the radio configuration
 *
 * @param The paramter to set
 * @param The desired parameter value
 */
void Radio_AT_Command::setParameter(ATS_REGISTERS reg, uint32_t value) {
	char buffer[255] = {0};
	char temp[10];

	// Make sure that the buffer is empty
	clearSerialBuffer();
	
	// Formulate the command string
	strcat(buffer, "ATS");
	strcat(buffer, itoa(reg, temp, 10));
	strcat(buffer, "=");
	strcat(buffer, itoa(value, temp, 10));
	strcat(buffer, "\r\n");

	// Send the command to the radio
	serialPort->write(buffer);

	// Make sure that the buffer is empty
	clearSerialBuffer();
}


/**
 * Set the debug message level
 *
 * @param The debug message level
 */
uint8_t Radio_AT_Command::setDebugMessage(DEBUG_MESSAGES msg) {
	// Make sure that the buffer is empty
	clearSerialBuffer();

	// Formulate and send the message
	switch(msg) {
	case NO_DEBUG:
		serialPort->write("AT&T\r\n");
		break;
	case RSSI:
		serialPort->write("AT&T=RSSI\r\n");
		break;
	case TDM:
		serialPort->write("AT&T=TDM\r\n");
		break;
	}

	// Make sure that the buffer is empty
	clearSerialBuffer();
}


void Radio_AT_Command::clearSerialBuffer() {
	while( serialPort->available() ) {
		serialPort->read();
	}
}
