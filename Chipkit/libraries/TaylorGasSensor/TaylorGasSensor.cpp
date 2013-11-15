//#include "../SoftSPI/SoftSPI.h"

#include <TaylorGasSensor.h>

/**
* Create the sensor object and initialize the hardware interface
*/
TaylorGasSensor::TaylorGasSensor(uint8_t csPin, uint8_t mosiPin, uint8_t misoPin, uint8_t sckPin) {
	this->spibus = new SoftSPI();

	// Store the pin information in local variables
	this->cs = csPin;
	this->mosi = mosiPin;
	this->miso = misoPin;
	this->sck = sckPin;

	// Set digital pin 8 to be an output for the SD card
	pinMode(8, OUTPUT);

	// Initialize the SPI bus
	this->spibus->begin(this->cs, this->mosi, this->miso, this->sck);
	this->spibus->setSpeed(2500000);						// Run the bus at 250kHz
	this->spibus->setDirection(SSPI_SHIFT_LEFT);			// Shift out the most significant bit first
	this->spibus->setMode(SSPI_MODE1);					// Have idle low SCK and data on rising clock edge
}



/**
* Read all the data channels in the sensor
*
* @return Data structure containing the read data
*/
TaylorGasSensor::channelData_t TaylorGasSensor::readAllData() {
	uint8_t i;

	// Variable to hold the data once it has been read
	TaylorGasSensor::channelDataWriter data;
	
	// Loop over every ADC channel in the sensor and read it
	for (i=0; i<9; i++) {
		data.array[i] = this->readChannel((TaylorGasSensor::channelNumbers) i);
	}

	// Return the populated data structure
	return(data.structure);
}



/**
 * Read a specific data channel
 *
 * @param The channel number (0-7)
 * @return The 12 bit value from the channel ADC
 */
uint16_t TaylorGasSensor::readChannel(channelNumbers channelNumber) {
	// SPI communication buffers
	uint8_t	receiveBuffer[2];
	uint8_t transmitBuffer[2];

	// Populate with a mask used to remove the channel number
	uint16_t receivedData = 0xF000;

	// Populate the transmit buffer with the appropriate frame
	transmitBuffer[0] = 0x04;
	transmitBuffer[1] = channelNumber;

	// Pull down the CS pin to activate the sensor comms lines
	this->spibus->setSelect(0);

	// Send the number of the desired data channel to the sensor
	this->spibus->transfer(2, transmitBuffer, receiveBuffer);

	// Receive the two byte response with the channel data, write 0x00 to it as
	// a garbage channel
	this->spibus->transfer(2, (uint8_t) 0x00, receiveBuffer);

	// Set CS pin to high to disable sensor comms lines
	this->spibus->setSelect(1);

	// Get rid of the channel number from the 1st 4 bits
	receivedData &= (receiveBuffer[1]<<8);
	receivedData &= receiveBuffer[0];

	return(receivedData);
}



/**
 * Reads the internal temperature of the sensor
 *
 * @return The temperature in Kelvin
 */
uint16_t TaylorGasSensor::readTemperature() {
	// Read the temperature channel
	return(this->readChannel(TaylorGasSensor::TEMPERATURE));	
}
