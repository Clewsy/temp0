#include "temp0.h"

// Interrupr sub-routine that is triggered by pressing the push-button.
void ISR_button(void)
{	// Button debounce.
	if( ((millis() - trigger_time) > BUTTON_DEBOUNCE) && (digitalRead(BUTTON_PIN)==LOW))
	{
		mode++;						// Increment to the next mode.
		if(mode>MODE_LARGE_INVERSE) {mode=MODE_NORMAL;}	// Rollover from the last mode to the first.
		trigger_time = millis();			// Reset the button debounce timer.
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

// Function to update the oled display.
void update_oled (double temp, double humi)
{
	if(mode != last_mode)		// If the mode has changed since last time.
	{
		display.clear_screen();	// Clear the screen.
		last_mode=mode;		// Update last_mode ready for next comparison.
	}

	display.send_command(OLED_INVERSE_DISABLE + (mode >> 1));	// I.e. Inverse enabled for modes 0b10 & 0b11

	switch(mode)
	{
		case MODE_NORMAL_INVERSE:						// Normal modes:
		case MODE_NORMAL:							//  _______________
			display.print_string((unsigned char *)"Temperature", 0, 0);	// |Temperature    |
			display.print_double(temp, 2, 0);				// |12.34°         |
			display.print_char(CHAR_INDEX_DEG, 2, 39);			// |         56.78%|
			display.print_char('C', 2, 47);					// |_______Humidity|
			display.print_double(humi, 4, 79);
			display.print_char('%', 4, 119);
			display.print_string((unsigned char *)"Humidity", 6, 63);
			break;

		case MODE_LARGE:							// Large modes:
		case MODE_LARGE_INVERSE:						//  _____
			display.print_large_double(temp, 0, 0);				// |12.3°|
			display.print_large_char(LARGE_CHAR_INDEX_DEG, 0, 96);		// |45.6%|
			display.print_large_double(humi, 4, 0);
			display.print_large_char('%', 4, 100);
			break;
	}
}

// Function will return the current value to output (analog) toi the external led.  I.e. the brightness level.
uint8_t get_pulse_value (void)
{
	switch(led_pulse_dir)	// Increment or decrement the value depending on the direction it's currently moving.
	{
		case GOING_UP:
			if (led_value>(255-LED_PULSE_SPEED))	{led_pulse_dir=GOING_DOWN;}
			else					{led_value+=LED_PULSE_SPEED;}
			break;
		case GOING_DOWN:
			if (led_value==0)			{led_pulse_dir=GOING_UP;}
			else					{led_value-=LED_PULSE_SPEED;}
			break;
	}

	return (led_value);
}

void setup(void) {

	// Initialize output pins.
	pinMode(LED_BUILTIN, OUTPUT);	// LED_BUILTIN - used to indicate when the Pro Trinket is booting/ready.
	pinMode(LED_EXTERNAL, OUTPUT);	// LED connected to output pin.

	// Turn on built-in LED to indicate Pro Trinket is "booting".
	digitalWrite(LED_BUILTIN, HIGH);

	// Initialise UART at 115200baud.
	Serial.begin(115200);

	// Initialise I2C comms and set speed.
	Wire.begin();
	Wire.setClock(400000);	//Default is 100kHz but devices in this system are capable of 400kHz.

	// Initialise sensor using the hdc1080 header and functions.
	sensor = hdc1080();
	sensor.reset();
	sensor.init();

	// Initialise oled using the ssd1306 header and functions.
	display = ssd1306();
	display.init();

display.print_string_arial((unsigned char *) "Game over.", 0, 0);
display.print_string_arial((unsigned char *) "!@#$%^&*(", 4, 0);
delay(5000);
display.clear_screen();

display.print_char_arial('#', 0, 0);
delay(5000);
display.clear_screen();
display.print_char_arial('$', 0, 0);
delay(5000);
display.clear_screen();
display.print_char_arial('!', 0, 0);
delay(5000);
display.clear_screen();
display.print_char_arial('A', 0, 0);
delay(5000);


	//////////////// Splash Screen Animation.
	display.test_pattern();
	delay(300);
	display.clear_screen();
	display.draw_box(0, 0, 8, 128);
	display.draw_box(1, 16, 6, 96);
	display.draw_box(2, 32, 4, 64);
	display.print_string((unsigned char *) "temp0", 3, 43);
	//////////////// End Splash Screen Animation.

	// Run heater while the splash screen is displayed.
	sensor.run_heater(5);
	display.clear_screen();

	// After all other initialisations are complete, enable the push-button interrupt.
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button, LOW);

	// Turn off built-in LED to indicate Pro Trinket has finished "booting".
	digitalWrite(LED_BUILTIN, LOW);
}

void loop(void)
{
	analogWrite(LED_EXTERNAL, get_pulse_value());			// Update the brightness of the external led.

	double *sensor_array = sensor.get_sensor_data();		// Update temperature and humidity readings.

	send_data(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Send temperature and humidity readings to the esp8266.

	update_oled(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Update the oled display with the latest  temperature and humidity readings.
}

