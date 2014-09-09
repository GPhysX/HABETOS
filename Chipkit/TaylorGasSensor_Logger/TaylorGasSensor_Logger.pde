//Gas Sensor Logger
//Code for polling information from the gas sensor by University of Florida and University of North Dakato
//Initial code by Ian McInerney
//Some edits by Matthew Nelson

//Revision
//Added some basic debug code
//Modified code for when we removed the GPS hardware
//Added Error LED indicator

#include <SD.h>
//#include <lea6.h>  //Removed GPS code
#include <SoftSPI.h>
#include <TaylorGasSensor.h>

// The CS pin for the SD card
#define SD_CS_PIN	8
#define SD_DEFAULT_CS_PIN	10

//Define a Debug mode, comment out this line if you wish to turn debug off
#define DEBUG

// The LED pin
#define LED_PIN		13
#define ERR_PIN         43

// The pins the sensor is on
#define CS_PIN		3
#define SCK_PIN		2
#define MOSI_PIN	6
#define MISO_PIN	5

// The filename of the datafile
//#define FILE_NAME "data.csv"

// The GPS object - Removed GPS hardware
//LEA6 *gps;

// The gas sensor object
TaylorGasSensor *gasSensor;

// The data file object
File dataFile;
char filename[] = "LOGGER00.CSV";

void setup() {
	// Configure the serial port and the LED
        #ifdef DEBUG
	Serial.begin(9600);
        #endif
	pinMode(LED_PIN, OUTPUT);
        pinMode(ERR_PIN, OUTPUT);
        //Test Error LED
        digitalWrite(ERR_PIN,HIGH);
        delay(500);
        digitalWrite(ERR_PIN,LOW);
        delay(500);
        digitalWrite(ERR_PIN,HIGH);
        delay(500);
        
        digitalWrite(ERR_PIN, LOW);

	// Initialize the GPS - Removed GPS hardware
	//gps = new LEA6(&Serial);
	//gps->init();
	
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
                #ifdef DEBUG
		Serial.print("\n\r");
		Serial.print("Error: No SD Card Detected\n\r");
                #endif
                digitalWrite(ERR_PIN, HIGH);
		return;
	}
	Serial.print("Completed\n\r");

//Here we create a new file each time we start up
//char filename[] = "LOGGER00.CSV";
for (uint8_t i=0; i< 100; i++) {
  filename[6] = i/10 + '0';
  filename[7] = i%10 + '0';
  if (! SD.exists(filename)) {
    // open a new file if none exists
    dataFile = SD.open(filename, FILE_WRITE);
    break;
  }
}

	// Open the file to write the data to
	Serial.print("Opening the data file...");
	dataFile = SD.open(filename, FILE_WRITE);
	if (!dataFile) {
		// Error opening the file
                #ifdef DEBUG
		Serial.print("\n\r");
		Serial.print("Error: Could not open ");
		Serial.print(filename);
                #endif
                digitalWrite(ERR_PIN, HIGH);
		return;
	}
        #ifdef DEBUG
	Serial.print("Opened ");
	Serial.print(filename);
	Serial.print("\n\r");
        #endif
	// Print headers into the file
        // Edited since we removed the GPS hardware
	//dataFile.print("Time, Lat, Long, Alt, Temp (K), Channel 0, Channel 1, Channel 2, Channel 3, Channel 4, Channel 5, Channel 6, Channel 7\n");
        dataFile.print("Temp (K), Channel 0, Channel 1, Channel 2, Channel 3, Channel 4, Channel 5, Channel 6, Channel 7\n");

	// Close the file to write the data to it
	dataFile.close();
}

void loop() {
	// The data structure to hold all of the channel data
	TaylorGasSensor::channelData_t allData;

	// The data structure to hold the GPS data
	//LEA6::ubloxData_t gpsData;

	// Wait for the GPS to provide data
	//while (!(gps->readGPS())) {
	//}
	//gpsData = gps->getPositionInfo();

	Serial.print("Taking a reading...");

	// Toggle the LED to indicate the read is in progress
	digitalWrite(LED_PIN, HIGH);

	// Read the data from all channels of the sensor
	allData = gasSensor->readAllData();

	// Open the log file
	dataFile = SD.open(filename, FILE_WRITE);
	
	// If the log file exists, write the data to it
	if (dataFile) {
		//dataFile.print(gpsData.time);
		//dataFile.print(",");
		//dataFile.print(gpsData.latitude);
		//dataFile.print(",");
		//dataFile.print(gpsData.longitude);
		//dataFile.print(",");
		//dataFile.print(gpsData.altitude);
		//dataFile.print(",");
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
        #ifdef DEBUG
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
        #endif

	// Toggle the LED to indicate the read is finished
	digitalWrite(LED_PIN, LOW);

	// Delay for 1 second before performing the next reading
//	delay(1000);
}
