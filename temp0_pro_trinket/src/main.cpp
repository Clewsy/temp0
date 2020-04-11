#include "temp0.h"

// Interrupr sub-routine that is triggered by pressing the push-button.
void ISR_button(void)
{	// Button debounce.
	if( ((millis() - trigger_time) > BUTTON_DEBOUNCE) && (digitalRead(BUTTON_PIN)==LOW))
	{
		mode++;							// Increment to the next mode.
		if(mode>MODE_TEMP_ONLY_INVERSE) {mode=MODE_NORMAL;}	// Rollover from the last mode to the first.
		trigger_time = millis();				// Reset the button debounce timer.
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
	temp_string[5]=0xb0;				// Append '°' to the string after the actual value.
	temp_string[6]='C';				// Append 'C' (Celcius) to the string after the ° symbol.
	temp_string[7]=0x00;				// Indicates end of character array.

	// Create an asci character string in the format - 12.34% - to indicate current humidity.
	unsigned char humi_string[7];			// Define a character array that will be printed as a string to show humidity : ##.##%
	dtostrf(humi, 5, 2, (char*)humi_string);	// Double-to-String function.  Syntax dtostrf(double, min_string_length, num_post_decimal_values, char_array_addr).
	humi_string[5]='%';				// Append '%' to the string after the actual value.
	humi_string[6]=0x00;				// Indicates end of character array.

	// Check if the display mode has changed.
	if(mode != last_mode)		// If the mode has changed since last time.
	{
		display.clear_screen();	// Clear the screen.
		last_mode=mode;		// Update last_mode ready for next comparison.
	}

	// Alternating display modes are inverted.
	display.send_command(OLED_INVERSE_DISABLE + (mode & 0b00000001));	// I.e. Inverse enabled by lsb of mode byte.

	switch(mode)
	{
		case MODE_NORMAL_INVERSE:								// Normal modes:
		case MODE_NORMAL:									//  _______________
			display.print_string((unsigned char *)"Temperature", Roboto_Black_12, 0, 0);	// |Temperature    |
			display.print_string((unsigned char *)temp_string, Roboto_Black_12, 2, 0);	// |12.34°C        |
			display.print_string((unsigned char*)humi_string, Roboto_Black_12, 4, 87);	// |         56.78%|
			display.print_string((unsigned char *)"Humidity", Roboto_Black_12, 6, 80);	// |_______Humidity|
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
	}												// |_________|
}

// Initialise internal timer #2.  This timer is used to trigger an ISR that controls the pulse effect of the external LED.
void init_timer(void)
{
	TIMSK2 |= (1 << TOIE2);	// Set Timer Overflow Interrupt Enable bit in the Timer 2 Interrupt Mask Register.
	TCCR2B |= (1 << CS02) | (1 << CS01) | (1 << CS00);	// Set divisor to 1024.
								// Overflow frequency = 1 / ((1/F)(2^8)(divisor))
								// With divisor set to 1024, overflow frequency ~ 61Hz.
}

// Timer 2 interrupt subroutine vector.  Used to pulse the external LED.
ISR(TIMER2_OVF_vect)
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
	
	analogWrite(LED_EXTERNAL, led_value);			// Update the brightness of the external led.
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

	//////////////// Splash Screen Animation.
	display.test_pattern();
	delay(300);
	display.clear_screen();
	display.draw_box(0, 0, 8, 128);
	display.draw_box(1, 16, 6, 96);
	display.draw_box(2, 32, 4, 64);
	display.print_string((unsigned char *) "temp0", Roboto_Black_12, 3, 45);
	//////////////// End Splash Screen Animation.

	// Run heater while the splash screen is displayed.
	sensor.run_heater(5);
	display.clear_screen();

	// Enable the timer interrupt (for LED pulsing).
	init_timer();

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

