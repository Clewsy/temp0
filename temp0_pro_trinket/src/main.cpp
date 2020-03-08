//#include <Arduino.h>
#include "temp0.h"

void setup() {

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);

	//Initialise UART at 115200baud.
	Serial.begin(115200);

	//Initialise sensor using the hdc1080 library.
	sensor = hdc1080();
	sensor.reset();
	sensor.init();
}

void loop() {

	digitalWrite(LED_BUILTIN, HIGH);	// turn the LED on (HIGH is the voltage level)
	delay(1000);				// wait for a second
	digitalWrite(LED_BUILTIN, LOW);		// turn the LED off by making the voltage LOW
	delay(200);				// wait for 200ms.

	float *sensor_array = sensor.get_sensor_data();

	Serial.print("t");
	Serial.print(sensor_array[TEMPERATURE]);
	Serial.print(";");
	Serial.print("h");
	Serial.print(sensor_array[HUMIDITY]);
	Serial.print(";");
}

