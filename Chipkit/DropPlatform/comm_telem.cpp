#include <ISU_Burner.h>
#include <commands.h>
//#include <Arduino.h>
#include <WProgram.h>

// The main serial interface
extern HardwareSerial *hardSerial;

// The telemetry control variable
extern short telemEnable;

void comm_telem( char* args ) {

	// Switch over the received argument
	switch ( atoi(args) ) {
	case 0:
		// Disable the telemtry
		hardSerial->println("Disabling Telemetry");
		telemEnable = 0;
		break;
	case 1:
		// Enable the telemetry
		hardSerial->println("Enabling Telemetry");
		telemEnable = 1;
		break;
	default:
		// Do nothing if the command is not recognized
		break;
	}
}
