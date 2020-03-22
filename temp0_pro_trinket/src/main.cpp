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
	display.test_pattern();
	for (uint8_t page=0;page<8;page++)
	{
		display.send_command(page);
		for (uint8_t col=0;col<128;col++)
		{
			display.send_command((col&0b1111), ((col>>4)+0b10000));
			display.set_address(page, col);
			display.send_data(0b00001111);
		}
	}

	delay(300);
	display.clear_screen();

	display.draw_box(0, 0, 8, 128);
	display.draw_box(1, 16, 6, 96);
	display.draw_box(2, 32, 4, 64);

	display.print_large_string((unsigned char *) "temp0", 3, 43);

	delay(4000);
	display.clear_screen();

////////////////end splash


}

void loop()
{

//	digitalWrite(LED_BUILTIN, HIGH);
//	delay(500);
//	digitalWrite(LED_BUILTIN, LOW);
	delay(500);

	double *sensor_array = sensor.get_sensor_data();

	Serial.print("t");
	Serial.print(sensor_array[TEMPERATURE]);
	Serial.print(";");
	Serial.print("h");
	Serial.print(sensor_array[HUMIDITY]);
	Serial.print(";");

	display.print_large_string((unsigned char *)"Temperature", 0, 0);
	display.print_large_double(sensor_array[TEMPERATURE], 2, 0);
	display.print_large_char(176, 2, 39);
	display.print_large_char('C', 2, 47);
	
	display.print_large_double(sensor_array[HUMIDITY], 4, 79);
	display.print_large_char('%', 4, 119);
	display.print_large_string((unsigned char *)"Humidity", 6, 63);

}

