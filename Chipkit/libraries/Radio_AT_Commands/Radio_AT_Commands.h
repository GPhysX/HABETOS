#ifndef RADIO_AT_COMMANDS_H_
#define RADIO_AT_COMMANDS_H_

#include <stdint.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * Sends and receives AT commands from an AT  capable radio.
 */
class Radio_AT_Command {

public:

	/**
	 * Possible debugging messages
	 */
	typedef enum DEBUG_MESSAGES {
		NO_DEBUG = 0,
		RSSI,
		TDM
	} DEBUG_MESSAGES;

	/**
	 * Possible values to read from the radio
	 */
	typedef enum ATI_VALUES {
		RADIO_VERSION = 1,
		BOARD_TYPE,
		BOARD_FREQUENCY,
		BOARD_VERSION,
		EEPROM,
		TDM_REPORT,
		RSSI_REPORT
	} ATI_VALUES;

	/**
	 * These are the registers able to be set using the ATS command
	 */
	typedef enum ATS_REGISTERS {
		EEPROM_FORMAT = 0,
		SERIAL_SPEED,
		AIR_SPEED,
		NETID,
		TXPOWER,
		ECC,
		MAVLINK,
		OP_RESEND,
		MIN_FREQ,
		MAX_FREQ,
		NUM_CHANNELS,
		DUTY_CYCLE,
		LBT_RSSI,
		MANCHESTER,
		RTSCTS,
		NODE_ID,
		NODE_DESTINATION,
		SYNC_ANY,
		NODE_COUNT
	} ATS_REGISTERS;

	/**
	 * The constructor for the AT command sender/parser
	 *
	 * @param radioSerialPort The object for the serial port where the radio is connected to
	 */
	Radio_AT_Command(HardwareSerial* radioSerialPort);

	/**
	 * Place the radio into AT command mode.
	 *
	 * @return 0 if entered command mode, 1 otherwise
	 */
	uint8_t startCommandMode();

	/**
	 * Exit the AT command mode
	 */
	void stopCommandMode();

	/**
	 * Reboot the radio
	 */
	void rebootRadio();

	/**
	 * Save the current parameters to EEPROM
	 */
	void saveParametersToEEPROM();

	/**
	 * Read an ATI value from the radio
	 *
	 * @param The ATI value to read from the radio
	 * @param Pointer to a character array containing the output
	 * @return The length of the returned output string
	 */
	int16_t readATI(ATI_VALUES atiToRead, char *result);

	/**
	 * Read a parameter from the radio configuration
	 *
	 * @param The parameter to read
	 * @return The value of the parameter
	 */
	int32_t readParameter(ATS_REGISTERS reg);

	/**
	 * Set a parameter in the radio configuration
	 *
	 * @param The paramter to set
	 * @param The desired parameter value
	 */
	void setParameter(ATS_REGISTERS reg, uint32_t value);

	/**
	 * Set the debug message level
	 *
	 * @param The debug message level
	 */
	uint8_t setDebugMessage(DEBUG_MESSAGES msg);


private:
	HardwareSerial *serialPort;


	void clearSerialBuffer();

};

#endif RADIO_AT_COMMANDS_H_
