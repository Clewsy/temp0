//#include <Arduino.h>
#include "temp0.h"

//Interrupr sub-routine that is triggered by pressing the push-button.
void ISR_button(void)
{
	if( ((millis() - trigger_time) > BUTTON_DEBOUNCE) && (digitalRead(BUTTON_PIN)==LOW))	//Button debounce.
	{
		mode++;										//Increment to the next mode.
		if(mode>MODE_WEIRD) {mode=MODE_NORMAL;}
		trigger_time = millis();							//Reset the button debounce timer.
	}
}

void setup(void) {

	//Initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);

	//Initialise UART at 115200baud.
	Serial.begin(115200);

	//Initialise sensor using the hdc1080 header and functions.
	sensor = hdc1080();
	sensor.reset();
	sensor.init();

	//Initialise oled using the ssd1306 header and functions.
	display = ssd1306();
	display.init();


	////////////////Splash Screen Animation
	display.test_pattern();
	delay(300);
	display.clear_screen();
	display.draw_box(0, 0, 8, 128);
	display.draw_box(1, 16, 6, 96);
	display.draw_box(2, 32, 4, 64);
	display.print_large_string((unsigned char *) "temp0", 3, 43);
	////////////////End Splash Screen Animation

	//Run heater while the splash screen is displayed.
	sensor.run_heater(5);
	display.clear_screen();

	//After all other initialisations zre complete, enable the push-button interrupt.
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button, LOW);
}

void loop(void)
{
	digitalWrite(LED_BUILTIN, mode);

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

