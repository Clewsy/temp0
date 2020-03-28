#include "temp0.h"

// Interrupr sub-routine that is triggered by pressing the push-button.
void ISR_button(void)
{
	if( ((millis() - trigger_time) > BUTTON_DEBOUNCE) && (digitalRead(BUTTON_PIN)==LOW))	//Button debounce.
	{
		mode++;										//Increment to the next mode.
		if(mode>MODE_WEIRD) {mode=MODE_NORMAL;}
		trigger_time = millis();							//Reset the button debounce timer.
	}
}

// Function to send temperature and humidity data as ascii strings to the ESP8266 over serial.
void send_data(double temp, double humi)
{
	// Send temperature string to esp8266.
	Serial.print("t");	// 't' indicates beginning of temperature string data.
	Serial.print(temp);	// Serial.print converts double to ascii string.
	Serial.print(";");	// ';' indicates end of data.

	// Send humidity string to esp8266.
	Serial.print("h");	// 'h' indicates beginning of humidity string data.
	Serial.print(humi);	// Serial.print converts double to ascii string.
	Serial.print(";");	// ';' indicates end of data.

}

//Function to update the oled display.
void update_oled (double temp, double humi)
{
	if(mode != last_mode)		// If the mode has changed since last time.
	{
		display.clear_screen();	// Clear the screen.
		last_mode=mode;		// Update last_mode ready for next comparison.
	}

	switch(mode)
	{
		case MODE_NORMAL:
			display.print_string((unsigned char *)"Temperature", 0, 0);
			display.print_double(temp, 2, 0);
			display.print_char(176, 2, 39);
			display.print_char('C', 2, 47);

			display.print_double(humi, 4, 79);
			display.print_char('%', 4, 119);
			display.print_string((unsigned char *)"Humidity", 6, 63);
			break;

		case MODE_WEIRD:
			display.print_large_double(temp, 0, 0);
			display.print_large_char(LARGE_CHAR_INDEX_DEG, 0, 96);
			display.print_large_double(humi, 4, 0);
			display.print_large_char('%', 4, 96);
			break;
	}
}

void setup(void) {

	// Initialize output pins.
	pinMode(LED_BUILTIN, OUTPUT);	// LED_BUILTIN - used to indicate when the Pro Trinket is booting/ready.
	pinMode(LED_EXTERNAL, OUTPUT);	// LED connected to output pin.

	// Turn on built-in LED to indicate Pro Trinket is "booting".
	digitalWrite(LED_BUILTIN, HIGH);

	// Initialise UART at 115200baud.
	Serial.begin(115200);

	// Initialise sensor using the hdc1080 header and functions.
	sensor = hdc1080();
	sensor.reset();
	sensor.init();

	// Initialise oled using the ssd1306 header and functions.
	display = ssd1306();
	display.init();


	//////////////// Splash Screen Animation
	display.test_pattern();
	delay(300);
	display.clear_screen();
	display.draw_box(0, 0, 8, 128);
	display.draw_box(1, 16, 6, 96);
	display.draw_box(2, 32, 4, 64);
	display.print_string((unsigned char *) "temp0", 3, 43);
	//////////////// End Splash Screen Animation

	// Run heater while the splash screen is displayed.
	sensor.run_heater(5);
	display.clear_screen();

	// After all other initialisations zre complete, enable the push-button interrupt.
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button, LOW);

	// Turn off built-in LED to indicate Pro Trinket has finished "booting".
	digitalWrite(LED_BUILTIN, LOW);
}

void loop(void)
{

	digitalWrite(LED_EXTERNAL, mode);

	double *sensor_array = sensor.get_sensor_data();		// Update temperature and humidity readings.

	send_data(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Send temperature and humidity readins to the esp8266.

	update_oled(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Send temperature and humidity readins to the esp8266.

}

