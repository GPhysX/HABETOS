#ifndef TAYLOR_GAS_SENSOR_H_
#define TAYLOR_GAS_SENSOR_H_

#include <stdint.h>

// Include the local copy of the SoftSPI library
#include "../SoftSPI/SoftSPI.h"


/**
 * This class is responsible for interfacing with the Gas Sensor.
 */
class TaylorGasSensor {

public:

	/*
	 * Structure to hold channel data for all the ADCs present on the sensor
	 */
	struct channelData_t {
		uint16_t channel0;
		uint16_t channel1;
		uint16_t channel2;
		uint16_t channel3;
		uint16_t channel4;
		uint16_t channel5;
		uint16_t channel6;
		uint16_t channel7;
		uint16_t temperature;
	};

	/*
	 * Enumerated list of all the channel numbers for the sensor
	 */
	enum channelNumbers {
		CHANNEL_0,
		CHANNEL_1,
		CHANNEL_2,
		CHANNEL_3,
		CHANNEL_4,
		CHANNEL_5,
		CHANNEL_6,
		CHANNEL_7,
		TEMPERATURE
	};
	
	/**
	 * Create the sensor object and initialize the hardware interface
	 */
	TaylorGasSensor(uint8_t csPin, uint8_t mosiPin, uint8_t misoPin, uint8_t sckPin);

	/**
	 * Read all the data channels in the sensor
	 *
	 * @return Data structure containing the read data
	 */
	channelData_t readAllData();

	/**
	 * Read a specific data channel
	 *
	 * @param The channel number (0-7)
	 * @return The 12 bit value from the channel ADC
	 */
	uint16_t readChannel(channelNumbers channelNumber);

	/**
	 * Reads the internal temperature of the sensor
	 *
	 * @return The temperature in Kelvin
	 */
	uint16_t readTemperature();



private:

	// The connections to the ChipKit for the sensor's wires
	uint8_t mosi;
	uint8_t miso;
	uint8_t sck;
	uint8_t cs;

	// The ChipKit software SPI library which will be used for this sensor
	SoftSPI spibus;

	// Union to assist in the easy reading of the data from teh sensor
	union channelDataWriter {
		uint16_t array[8];
		channelData_t structure;
	};


};

#endif
