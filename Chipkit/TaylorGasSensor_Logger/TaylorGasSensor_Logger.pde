#include <SD.h>

#include "TaylorGasSensor.h"

#define CS_PIN		3
#define SCK_PIN		2
#define MOSI_PIN	6
#define MISO_PIN	5


TaylorGasSensor *gasSensor;


void setup() {
	gasSensor = new TaylorGasSensor(CS_PIN, MOSI_PIN, MISO_PIN, SCK_PIN);
	Serial.begin(9600);
}

void loop() {
	
}
