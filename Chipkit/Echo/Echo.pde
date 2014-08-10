

void setup() {
	Serial3.begin(9600);
}


void loop() {
	while (Serial3.available()) {
		Serial3.write(Serial3.read());
	}
}
