#include <ISU_Burner.h>

#define FALSE	0
#define TRUE	1

ISU_Burner* burner1;

void setup() {
	Serial.begin(9600);
	Serial.println("Starting burner program");
	burner1 = new ISU_Burner(2, 9);
	Serial.println("Complete");
}

void loop() {

	delay(1000);
	Serial.println("Sending Burn");
	burner1->sendBurn(3);
	Serial.println("Done Burning");
}
