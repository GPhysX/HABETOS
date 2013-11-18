#include <SD.h>
#include <SoftSPI.h>
#include <TaylorGasSensor.h>

// The CS pin for the SD card
#define SD_CS_PIN	10

// The pins the sensor is on
#define CS_PIN		3
#define SCK_PIN		2
#define MOSI_PIN	6
#define MISO_PIN	5

// The filename of the datafile
const char* fileName = "gasSensorData.csv";

// The gas sensor object
TaylorGasSensor *gasSensor;

// The data file object
File dataFile;

void setup() {
	Serial.begin(9600);

	Serial.print("Initializing Gas Sensor...");
	gasSensor = new TaylorGasSensor(CS_PIN, MOSI_PIN, MISO_PIN, SCK_PIN);
	Serial.print("Completed\n\r");

	Serial.print("Initializing SD Card...");
	// Set the CS pin to be an output
	pinMode(SD_CS_PIN, OUTPUT);

	// Start the SD card
	if (!SD.begin(SD_CS_PIN)) {
		// The card is not present
		Serial.print("\n\r");
		Serial.print("Error: No SD Card Detected");
		return;
	}
	Serial.print("Completed\n\r");

	// Open the file to write the data to
	Serial.print("Opening the data file...");
	dataFile = SD.open(fileName, FILE_WRITE);
	if (!dataFile) {
		// Error opening the file
		Serial.print("\n\r");
		Serial.print("Error: Could not open ");
		Serial.print(fileName);
	}
	Serial.print("Opened ");
	Serial.print(fileName);
	Serial.print("\n\r");

	// Print headers into the file
	dataFile.print("Temp (K), Channel 0, Channel 1, Channel 2, Channel 3, Channel 4, Channel 5, Channel 6, Channel 7\n");
}

void loop() {
	TaylorGasSensor::channelData_t allData;

	Serial.print("Taking a reading...");
	// Read the data from all channels of the sensor
	allData = gasSensor->readAllData();

	// Store the data on the memory card in a comma-delimited manner
	dataFile.print(allData.temperature);
	dataFile.print(",");
	dataFile.print(allData.channel0);
	dataFile.print(",");
	dataFile.print(allData.channel1);
	dataFile.print(",");
	dataFile.print(allData.channel2);
	dataFile.print(",");
	dataFile.print(allData.channel3);
	dataFile.print(",");
	dataFile.print(allData.channel4);
	dataFile.print(",");
	dataFile.print(allData.channel5);
	dataFile.print(",");
	dataFile.print(allData.channel6);
	dataFile.print(",");
	dataFile.print(allData.channel7);
	dataFile.print("\n");

	Serial.print("Completed\n\r");
	// Delay for 1 second before performing the next reading
	delay(1000);
}
