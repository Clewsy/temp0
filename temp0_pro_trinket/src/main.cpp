//#include <Arduino.h>
#include "temp0.h"



	hdc1080 sensor;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);

	Wire.begin();

	sensor = hdc1080();
	sensor.reset();
	sensor.init();
}

// the loop function runs over and over again forever
void loop() {
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);                       // wait for a second
	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
	delay(200);                       // wait for a second






////////////////////
//Wire.beginTransmission(HDC1080_I2C_ADDRESS);
////Wire.write(0b10000000);
//Wire.write(HDC1080_ADDRESS_TEMP);
////Wire.write(0b10000001);
//Wire.endTransmission(HDC1080_I2C_ADDRESS);
//delay(20);
//Wire.requestFrom(HDC1080_I2C_ADDRESS, 2);
//uint8_t temp_msb = Wire.read();
//uint8_t temp_lsb = Wire.read();
//uint16_t temp_data = ( (temp_msb << 8) | temp_lsb);
//float temp = ((float)temp_data);
//temp=temp*165;
//temp= (temp / 65536);
//temp = (temp-40);
/////////////////////////


	float *sensor_array = sensor.get_sensor_data();


	Serial.print("temp");

	Serial.print(sensor_array[0]);
	Serial.print(" ");
	Serial.print(sensor_array[1]);
	Serial.print(" ");
//	Serial.print(temp, 2);

	Serial.print(" 234;");
	Serial.print("humidity");
	Serial.print("80;");
}

