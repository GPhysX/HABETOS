#include <SD.h>
#include <SoftSPI.h>
#include <TaylorGasSensor.h>

// The CS pin for the SD card
#define SD_CS_PIN	8
#define SD_DEFAULT_CS_PIN	10

// The LED pin
#define LED_PIN		13

// The pins the sensor is on
#define CS_PIN		3
#define SCK_PIN		2
#define MOSI_PIN	6
#define MISO_PIN	5

// The filename of the datafile
#define FILE_NAME "data.csv"

// The gas sensor object
TaylorGasSensor *gasSensor;

// The data file object
File dataFile;

void setup() {
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);

	Serial.print("Initializing Gas Sensor...");
	gasSensor = new TaylorGasSensor(CS_PIN, MOSI_PIN, MISO_PIN, SCK_PIN);
	Serial.print("Completed\n\r");

	Serial.print("Initializing SD Card...");
	// Set the CS pin to be an output
	pinMode(SD_CS_PIN, OUTPUT);
	digitalWrite(SD_CS_PIN, HIGH);

	pinMode(SD_DEFAULT_CS_PIN, OUTPUT);
	digitalWrite(SD_DEFAULT_CS_PIN, HIGH);

	// Start the SD card
	if (!SD.begin(SD_CS_PIN)) {
		// The card is not present
		Serial.print("\n\r");
		Serial.print("Error: No SD Card Detected\n\r");
		return;
	}
	Serial.print("Completed\n\r");

	// Open the file to write the data to
	Serial.print("Opening the data file...");
	dataFile = SD.open(FILE_NAME, FILE_WRITE);
	if (!dataFile) {
		// Error opening the file
		Serial.print("\n\r");
		Serial.print("Error: Could not open ");
		Serial.print(FILE_NAME);
		return;
	}
	Serial.print("Opened ");
	Serial.print(FILE_NAME);
	Serial.print("\n\r");

	// Print headers into the file
	dataFile.print("Temp (K), Channel 0, Channel 1, Channel 2, Channel 3, Channel 4, Channel 5, Channel 6, Channel 7\n");

	// Close the file to write the data to it
	dataFile.close();
}

void loop() {
	// The data structure to hold all of the channel data
	TaylorGasSensor::channelData_t allData;

	Serial.print("Taking a reading...");

	// Toggle the LED to indicate the read is in progress
	digitalWrite(LED_PIN, HIGH);

	// Read the data from all channels of the sensor
	allData = gasSensor->readAllData();

	// Open the log file
	dataFile = SD.open(FILE_NAME, FILE_WRITE);
	
	// If the log file exists, write the data to it
	if (dataFile) {
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

		// The file must be closed in order for the data to be written
		dataFile.close();
	}

	// Print the data to the computer terminal as well
	Serial.print("\n\r");
	Serial.print(allData.temperature);
	Serial.print(",");
	Serial.print(allData.channel0);
	Serial.print(",");
	Serial.print(allData.channel1);
	Serial.print(",");
	Serial.print(allData.channel2);
	Serial.print(",");
	Serial.print(allData.channel3);
	Serial.print(",");
	Serial.print(allData.channel4);
	Serial.print(",");
	Serial.print(allData.channel5);
	Serial.print(",");
	Serial.print(allData.channel6);
	Serial.print(",");
	Serial.print(allData.channel7);
	Serial.print("\n");

	Serial.print("Completed\n\r");

	// Toggle the LED to indicate the read is finished
	digitalWrite(LED_PIN, LOW);

	// Delay for 1 second before performing the next reading
	delay(1000);
}
