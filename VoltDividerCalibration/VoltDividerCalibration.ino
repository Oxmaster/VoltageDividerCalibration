/* Calibrate voltage divider ratio by Oxmaster
 *
 * Change resolution, voltage range and analog pin before running
 *
 * you will need a multimeter or a voltmeter to confirm your readings
 *
 * open serial port at 115200 baud and proceed
 * with instructions found in the serial terminal
*/

// how many bits does the ADC return
const uint32_t resolution = 1024;

// max voltage that ADC can read
const float voltageMax = 1.0;

// pin to read data from
const uint16_t analogPin = A0;


// -------- vars ----------------------------------------------------
// do not change

// how many readouts to perform
const uint8_t reps = 5;

const float bitPerVolt = voltageMax / (float)resolution;
float voltageDividerRatio = 1;
float average = 0;

void setup() {
	Serial.begin(115200);
	
	readADC();

	Serial.println();
	Serial.print("Averaged readout was: ");
	Serial.print(average, 3);
	Serial.print("V, what does your voltmeter say? (write a float ex. 3.7): ");

}

void loop() {
	adjustRatio();
}


void readADC(){
	static float volts = 0;
	uint32_t adcOutput = 0;

	average = 0;
	
	Serial.println();
	Serial.print("Performing ");
	Serial.print(reps);
	Serial.println(" voltage readouts");
	
	for (uint8_t i = 0; i < reps; i++){
		
		adcOutput = analogRead(analogPin);
		volts = (float)adcOutput * bitPerVolt * voltageDividerRatio;
		
		Serial.print(volts, 3);
		Serial.print("V\t\t");
		
		average += volts;
		
		delay(5);
	}
	average /= reps;
	
	Serial.println();
	Serial.print("Average readout: ");
	Serial.print(average, 3);
	Serial.print("V");
}

void adjustRatio(){
	while (Serial.available() > 0) {
		float userData = Serial.parseFloat();
		Serial.print(userData);

		// flush the input
		while(Serial.available() > 0) {
			char t = Serial.read();
		}

		voltageDividerRatio = (userData / average) * voltageDividerRatio;

		Serial.println();
		Serial.println();
		Serial.print("Calculated voltage divider ratio: ");
		Serial.print(voltageDividerRatio, 4);
		Serial.println();

		readADC();

		Serial.println();
		Serial.println();
		Serial.println("End of program, you can change the voltage and reset the sketch to verify\n"
		"if the ratio is the same for different potentials");

		while(1){
			// prevent watchdog from triggering
			delay(500);
		}
	}
}
