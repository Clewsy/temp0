#include "temp0.h"

// Configure the timer interrupt (for LED pulsing).
void led_pulse_init(void)
{
	TCCR2B |= (1 << CS02) | (1 << CS01) | (1 << CS00);	// Set divisor to 1024.
								// Overflow frequency = 1 / ((1/F)(2^8)(divisor))
}								// With divisor set to 1024, overflow frequency ~ 61Hz.

// Enable (true) or disable (false) the pulsing LED.
void led_pulse(bool enable)
{
	analogWrite(LED_EXTERNAL, 0);			// If enabling or disabling the LED pulse, always set the analogue brightness value to zero.

	if(enable)	{TIMSK2 |= (1 << TOIE2);}	// Set Timer Overflow Interrupt Enable bit in the Timer 2 Interrupt Mask Register.
	else		{TIMSK2 &= ~(1 << TOIE2);}	// Clear Timer Overflow Interrupt Enable bit in the Timer 2 Interrupt Mask Register.
}

// Timer 2 interrupt subroutine vector.  Used to pulse the external LED.
ISR(TIMER2_OVF_vect)
{
	led_value = (led_value + (led_pulse_dir * LED_PULSE_SPEED));						// Increment or decrement led_value by LED_PULSE_SPEED.

	if ((led_value>(LED_MAX_VALUE-LED_PULSE_SPEED)) | (led_value==0)) {led_pulse_dir = -led_pulse_dir;}	// Reverse direction at either extreme of led_value.

	analogWrite(LED_EXTERNAL, led_value);									// Update the brightness of the external led.
}

// Interrupr sub-routine that is triggered by pressing the push-button.
void ISR_button(void)
{
	// Button debounce.
	if( ((millis() - trigger_time) > BUTTON_DEBOUNCE) && (digitalRead(BUTTON_PIN)==LOW))
	{
		mode++;				// Increment to the next mode.
		if(mode>MODE_BLANK) {mode=0;}	// Rollover from the last mode to the first.
		trigger_time = millis();	// Reset the button debounce timer.
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
	// Create an asci character string in the format - 12.34°C - to indicate current temperature.
	unsigned char temp_string[8];			// Define a character array that will be printed as a string to show temperature : ##.##°C
	dtostrf(temp, 5, 2, (char*)temp_string);	// Double-to-String function.  Syntax dtostrf(double, min_string_length, num_post_decimal_values, char_array_addr).
	temp_string[5]=0xB0;				// Append '°' to the string after the actual value.
	temp_string[6]='C';				// Append 'C' (Celcius) to the string after the ° symbol.
	temp_string[7]=0x00;				// Indicates end of character array.

	// Create an asci character string in the format - 56.78% - to indicate current humidity.
	unsigned char humi_string[7];			// Define a character array that will be printed as a string to show humidity : ##.##%
	dtostrf(humi, 5, 2, (char*)humi_string);	// Double-to-String function.  Syntax dtostrf(double, min_string_length, num_post_decimal_values, char_array_addr).
	humi_string[5]='%';				// Append '%' to the string after the actual value.
	humi_string[6]=0x00;				// Indicates end of character array.

	// Check if the display mode has changed.
	if(mode != last_mode)		// If the mode has changed since last time.
	{
		display.clear_screen();	// Clear the screen.

		if (mode>=MODE_BLANK)	{led_pulse(false);}	// If changed to blank screen mode, disable LED pulse.
		else			{led_pulse(true);}	// Otherwise, enable LED pulse.

		last_mode=mode;		// Update last_mode for next comparison.
	}

	// Alternating display modes are inverted.
	display.invert_screen(mode & 0b00000001);	// I.e. Inverse enabled by lsb of mode byte.

	switch(mode)
	{
		case MODE_NORMAL:									// Normal modes:
		case MODE_NORMAL_INVERSE:								//  _______________
			display.print_string((unsigned char*)"Temperature", Roboto_Black_12, 0, 0);	// |Temperature    |
			display.print_string((unsigned char*)temp_string, Roboto_Black_12, 2, 0);	// |12.34°C        |
			display.print_string((unsigned char*)humi_string, Roboto_Black_12, 4, 87);	// |         56.78%|
			display.print_string((unsigned char*)"Humidity", Roboto_Black_12, 6, 80);	// |_______Humidity|
			break;

		case MODE_LARGE:									// Large modes:
		case MODE_LARGE_INVERSE:								//  _________
			display.print_string((unsigned char*)temp_string, Roboto_Black_26, 0, 16);	// | 12.34°C |
			display.print_string((unsigned char*)humi_string, Roboto_Black_26, 4, 20);	// |_56.78_%_|
			break;

		case MODE_TEMP_ONLY:									// Temp only modes:
		case MODE_TEMP_ONLY_INVERSE:								//  _________
			display.print_string((unsigned char*)temp_string, Roboto_Black_26, 2, 16);	// |         |
			break;										// | 12.34°C |
													// |_________|
		case MODE_LOGO_CLEWS:			// No data readout, just show clews logo.
		case MODE_LOGO_CLEWS_INVERSE:
			display.map_bits(logo_clews);
			break;
		case MODE_LOGO_HAD:			// No data readout, just show HackADay logo.
		case MODE_LOGO_HAD_INVERSE:
			display.map_bits(logo_had);
			break;
		case MODE_BLANK:			// Blank screen.
			display.clear_screen();
	}
}


void setup(void) {

	// Initialize output pins.
	pinMode(LED_BUILTIN, OUTPUT);	// LED_BUILTIN - used to indicate when the Pro Trinket is booting/ready.
	pinMode(LED_EXTERNAL, OUTPUT);	// External LED connected to a output pin.

	// Turn on built-in LED to indicate Pro Trinket is "booting".
	digitalWrite(LED_BUILTIN, HIGH);

	// Initialise UART at 115200baud.
	Serial.begin(115200);

	// Initialise I2C comms and set speed.
	Wire.begin();
	Wire.setClock(400000);	//Default is 100kHz but devices in this system are capable of 400kHz.

	// Initialise sensor using the hdc1080 header and functions.
	sensor = hdc1080();
	sensor.init();

	// Initialise oled using the ssd1306 header and functions.
	display = ssd1306();
	display.init();

	//////////////// Splash Screen Animation.
	display.map_bits(logo_clews);
	delay(750);
	display.map_bits(logo_had);
	delay(750);
	display.test_pattern();
	delay(250);
	display.clear_screen();
	display.draw_box(0, 0, 8, 128);
	display.print_string((unsigned char*)"temp0", Roboto_Black_12, 3, 45);
	//////////////// End Splash Screen Animation.

	// Run heater for 5 seconds while the splash screen is displayed.
	sensor.run_heater(5);
	display.clear_screen();

	// Initialise the timer/counter interrupt that will control the pulsing LED, then enable pulsing.
	led_pulse_init();
	led_pulse(true);

	// Turn off built-in LED to indicate Pro Trinket has finished "booting".
	digitalWrite(LED_BUILTIN, LOW);

	// After all other initialisations are complete, enable the push-button interrupt.
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button, LOW);
}

void loop(void)
{
	double *sensor_array = sensor.get_sensor_data();		// Update temperature and humidity readings.

	send_data(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Send temperature and humidity readings to the esp8266 (converts double to string).

	update_oled(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Update the oled display with the latest  temperature and humidity readings.
}

