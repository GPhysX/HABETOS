#include <lea6.h>
#include <Radio_AT_Commands.h>

LEA6 *gps;
HardwareSerial *hardSerial;
Radio_AT_Command *port0;

char gpsData[255];
uint8_t strLength = 0;
int32_t parameter = -1;
char atiValue[255];

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);

	Serial2.println("Initializing AT Command Interface");
	port0 = new Radio_AT_Command(&Serial2);
	Serial2.println("Initialized AT Command Interface");

/*
	Serial.println(port0->startCommandMode(), DEC);
	parameter = port0->readParameter(Radio_AT_Command::MIN_FREQ);
	port0->readATI(Radio_AT_Command::RSSI_REPORT, atiValue);
	port0->stopCommandMode();
	
	Serial.print("Parameter: ");
	Serial.print(parameter, DEC);
	Serial.print("\n\r");

	Serial.print("RSSI Report: ");
	Serial.print(atiValue);
	Serial.print("\n\r");
*/

	Serial.println("Initializing GPS");
	gps = new LEA6(&Serial3);

	// Initialize the GPS and check for reversal of the wires
	if ( gps->init() ) {
		Serial2.println("GPS Wires are reversed");
	} else {
		Serial2.println("GPS Configured Successfully");
	}

}


void loop() {
	uint8_t radioStatus = 0;
	strLength = 0;

	if ( gps->readGPS() ) {
//		Serial.println("Reading GPS");
		strLength = gps->getPositionString(gpsData);

		if (strLength) {
			radioStatus = port0->startCommandMode();
			if ( radioStatus ) {
				// The radio didn't enter command mode
//				Serial.println("Error getting RSSI: Couldn't enter AT mode");
			} else {
				// The radio is in command mode, get the RSSI
//				Serial.println("Getting RSSI");
				port0->readATI(Radio_AT_Command::RSSI_REPORT, atiValue);
				port0->stopCommandMode();
			}
//			Serial.println("Sending Data");
			Serial2.print("#");			// Start character
			Serial2.print(gpsData);		// GPS information
			Serial2.print(",");			// Place a delimiter
			Serial2.print(atiValue);	// RSSI information
			Serial2.print("$");			// Stop character
			Serial2.print("\r\n");
		}
	}

	// Wait 1 second
	delay(1000);
}

