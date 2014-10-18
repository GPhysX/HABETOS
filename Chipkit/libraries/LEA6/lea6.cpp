/**
 * Source file containing the functions for the LEA6 GPS interface class.
 * 
 * Authored by Ian McInerney, Iowa State University
 */
#include "lea6.h"
#include "TinyGPS.h"

/**
 * Standard C++ constructor function.
 * This function does not do anything, to configure the GPS you must use the init() function
 */
LEA6::LEA6(HardwareSerial *serialPort) {
	this->serialPort = serialPort;
}
 
/**
 * Function to perform initial configuration for the uBlox GPS.
 * It will open the serial port to be 38400BPS, and then configure the UBlox GPS to output only GGA and RMC NMEA strings on its UART port
 *
 * @return 0 if the port is configured correctly, 1 if the serial wires are swapped
 */
uint8_t LEA6::init(void) {
	uint8_t timeout = 500;

	// Open the serial port which the GPS is attached to
    this->serialPort->begin(38400);

	// Verify if the serial wires are connected properly
	this->serialPort->write("$PUBX,04*37\r\n");
	while ( !serialPort->available() && timeout ) {
		delay(1);
		timeout--;
	}
	if ( !serialPort->available() ) {
		// No data was received, so report back saying the wires are crossed
		return(1);
	}

	// Enable NMEA protocol output on GPS serial port
	this->serialPort->write("$PUBX,41,1,0007,0002,38400,0*21\r\n");
	
	// Set the NMEA output rate on the GPS serial port to be every epoch, and off for every other port
	this->serialPort->write("$PUBX,40,GGA,0,1,0,0,0,0*5B\r\n");
	this->serialPort->write("$PUBX,40,RMC,0,1,0,0,0,0*46\r\n");
	
	// Initialize the delimiter to be a comma
	this->delimiter[0] = ',';
	
	return(0);
}

/**
 * Function to retrieve the current GPS data
 * 
 * @return A copy of the UBLOX_RECEIVED_INFO object with the most recent GPS data
 */
LEA6::ubloxData_t LEA6::getPositionInfo() {
	return(info);
}

/**
 * Function to set the delimiter for the ASCII string data
 *
 * @param The single ASCII character to use as the delimiter
 */
void LEA6::setStringDelimiter(char *newDelimiter) {
	this->delimiter[0] = newDelimiter[0];
}

/**
 * Function to retrieve the current GPS data as an ASCII string
 *
 * @param positionString A pointer to a character array to store the generated string in
 * @return The length of the string, or 0 if there is an error
 */
uint8_t LEA6::getPositionString(char *positionString) {
	char numberString[20];
	
	// Start with the time of the GPS fix (use it to blank the string)
	strcpy(positionString, itoa(info.time, numberString, 10));
	strcat(positionString, delimiter);
	
	// Next put the latitude and longitude into the string
	strcat(positionString, itoa(info.latitude, numberString, 10));
	strcat(positionString, delimiter);
	strcat(positionString, itoa(info.longitude, numberString, 10));
	strcat(positionString, delimiter);

	// Next include the altitude
	strcat(positionString, itoa(info.altitude, numberString, 10));
	strcat(positionString, delimiter);

	// Next include the number of satellites and the HDOP
	strcat(positionString, itoa(info.satellites, numberString, 10));
	strcat(positionString, delimiter);
	strcat(positionString, itoa(info.hdop, numberString, 10));

	return(strlen(positionString));
}

/**
 * Function to poll the GPS and then parse the returned data into the GPS data structure.
 * This function does not return the data, to retrieve the data you must call the
 * getPositionInfo() function.
 * 
 * @return Returns true if new GPS data is present in the data structure
 */
bool LEA6::readGPS() {
	// We don't care about some of the variables the parsing creates, but we must have a variable for them to pass to functions
	unsigned long gar_date;
	unsigned long gar_age;
	
	unsigned long time;		// The time which the GPS received reports
	bool parsed = false;	// Whether or not the GPS data has been successfully parsed into the data array

	// Loop to receive the GPS serial data
    while (this->serialPort->available()) {
		if (parser.encode(serialPort->read())) {
			// Returns true if the GPS string passed into the TinyGPS library was decoded completely
			parser.get_datetime(&gar_date, &time, &gar_age);	// Pull the current time of the GPS string from the parser
			if (time != lastTime) {
				// Only continue copying the data if this is a new GPS string
				parser.get_position(&(info.latitude), &(info.longitude), &(info.fixAge));
				parser.get_datetime(&gar_date, &(info.time), &gar_age);
				info.time = time;
				info.altitude = parser.altitude();
				info.satellites = parser.satellites();
				info.hdop = parser.hdop();
				
				lastTime = time;
				parsed = true;
			}
		}
	}
    return parsed;
}
