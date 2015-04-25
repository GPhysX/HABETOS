#include <SD.h>
#include <utilities.h>

char basename[] = "HELLA";
char extension[] = ".CSV";
char finalname[13];
File test;

void setup() {
	Serial.begin(9600);

	if ( !SD.begin(8) ) {
		Serial.println("Unable to open SD card");
		return;
	}
	Serial.println("SD Initialized");
	
	if ( findNextFilename(&SD, basename, extension, finalname, 50) )
		Serial.println("Unable to find a file");
	else {
		Serial.print("Found file: ");
		Serial.print(finalname);
		Serial.print("\n\r");
	}

	test = SD.open(finalname, FILE_WRITE);
	test.close();


}


void loop() {
//	while (Serial.available()) {
//		Serial.write(Serial.read());
//	}
}
