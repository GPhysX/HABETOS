#include <ISU_Burner.h>
#include <lea6.h>
#include "commands.h"

// The GPS attached to the payload
LEA6 *gps;

// The main serial interface
HardwareSerial *hardSerial;

// The burners
ISU_Burner *burner1;
ISU_Burner *burner2;
ISU_Burner *burner3;

// Control the telemetry (enabled from the start)
short telemEnable = 1;

void setup(void) {
	// Create the commands
	commands_init();

	// Create the GPS object on Serial1
	gps = new LEA6(&Serial1);
	gps->init();

	// Prepare the serial port for the telemetry transmitter
	Serial.begin(9600);
	hardSerial = &Serial;

	// Create the three burners
	Serial.print("Initializing the three burners...");
	burner1 = new ISU_Burner(7, 8);
	burner2 = new ISU_Burner(9, 10);
	burner3 = new ISU_Burner(11, 12);
	Serial.print("Completed\n\r");
}


void loop(void) {
	// The data structure to hold the GPS data
	LEA6::ubloxData_t gpsData;
	char comm_data[2];
	
	// Read the telemetry serial port and have it decipher the command
	while ( Serial.available() > 0 ) {
		comm_data[0] = Serial.read();
		comm_data[1] = 0;
		commands_interpret(comm_data);
	}

	// See if the GPS can provide data
	if ( gps->readGPS()  && telemEnable) {
		// If it can, then print a telemetry string
		gpsData = gps->getPositionInfo();

		Serial.print(gpsData.time);
		Serial.print(",");
		Serial.print(gpsData.latitude);
		Serial.print(",");
		Serial.print(gpsData.longitude);
		Serial.print(",");
		Serial.print(gpsData.altitude);
		Serial.print(",");
		Serial.print(gpsData.satellites);
		Serial.print("\n\r");
	}
}
