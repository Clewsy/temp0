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
	sensor.run_heater(1);

	display = ssd1306();
	display.init();


////////////////splash
	for (uint8_t page=0;page<8;page++)
	{
		display.send_command(page);
		for (uint8_t col=0;col<128;col++)
		{
			display.send_command((col&0b1111), ((col>>4)+0b10000));
			display.set_address(page, col);
			display.send_data(0b11001100);
	//		display.send_data(0b00001111);
		}
	}

	delay(1000);
	display.clear_screen();

	display.draw_box(0, 0, 8, 128);
	display.draw_box(1, 16, 6, 96);
	display.draw_box(2, 32, 4, 64);
	display.draw_box(3, 48, 2, 32);
////////////////end splash


}
uint8_t c = 0;
void loop() {

	digitalWrite(LED_BUILTIN, HIGH);	// turn the LED on (HIGH is the voltage level)
	display.send_command(OLED_CONTRAST, 255);

	delay(500);				// wait for a second

	digitalWrite(LED_BUILTIN, LOW);		// turn the LED off by making the voltage LOW
	display.send_command(OLED_CONTRAST, 127);


	delay(500);				// wait for 200ms.



	float *sensor_array = sensor.get_sensor_data();

	Serial.print("t");
	Serial.print(sensor_array[TEMPERATURE]);
//	Serial.print(sensor.read_config_register());
	Serial.print(";");
	Serial.print("h");
	Serial.print(sensor_array[HUMIDITY]);
	Serial.print(";");
}

