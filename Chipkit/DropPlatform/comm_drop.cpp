#include <ISU_Burner.h>
#include <commands.h>
//#include <Arduino.h>
#include <WProgram.h>

// The main serial interface
extern HardwareSerial *hardSerial;

// The burners
extern ISU_Burner *burner1;
extern ISU_Burner *burner2;
extern ISU_Burner *burner3;

void comm_drop( char* args ){
	hardSerial->println("Dropping Plane");
	// Start the burn
	burner1->writeLogicLevel(1);
	burner2->writeLogicLevel(1);
	burner3->writeLogicLevel(1);
	digitalWrite(13, 1);

	// Wait for the burn time
	delay(5000);

	// Stop the burn
	burner1->writeLogicLevel(0);
	burner2->writeLogicLevel(0);
	burner3->writeLogicLevel(0);
	digitalWrite(13, 0);
	hardSerial->println("Drop Complete");
}
