#include <ISU_Burner.h>

#define FALSE	0
#define TRUE	1

ISU_Burner* burner1;

void setup() {
	Serial.begin(9600);
	Serial.println("Starting burner program");
	burner1 = new ISU_Burner(2, 9);
	burner1->writeLogicLevel(0);
	Serial.println("Complete");
}

void loop() {

	delay(4000);
	Serial.println("Sending Burn");
	burner1->writeLogicLevel(1);
	delay(3000);
	burner1->writeLogicLevel(0);
	Serial.println("Done Burning");
}
