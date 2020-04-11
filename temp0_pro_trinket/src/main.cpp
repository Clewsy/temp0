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
		case MODE_NORMAL_INVERSE:
		case MODE_NORMAL:
			display.print_string((unsigned char *)"Temperature", Roboto_Mono_12, 0, 0);	// Normal modes:
			display.print_double(temp, Roboto_Mono_12, 2, 0);				//  _______________
			display.print_char(0xB0, Roboto_Mono_12, 2, 36); // 0xB0:'°'			// |Temperature    |
			display.print_char('C', Roboto_Mono_12, 2, 43);					// |12.34°         |
			display.print_double(humi, Roboto_Mono_12, 4, 86);				// |         56.78%|
			display.print_char('%', Roboto_Mono_12, 4, 121);				// |_______Humidity|
			display.print_string((unsigned char *)"Humidity", Roboto_Mono_12, 6, 72);
			break;

		case MODE_LARGE:
		case MODE_LARGE_INVERSE:
			display.print_double(temp, Roboto_Mono_25, 0, 0+16);				// Large modes:
			display.print_char(0xB0, Roboto_Mono_25, 0, 73+16); // 0xB0:'°'			//  _______
			display.print_char('C', Roboto_Mono_25, 0, 86+16);				// |12.34°C|
			display.print_double(humi, Roboto_Mono_25, 4, 0+19);				// |56.78_%|
			display.print_char('%', Roboto_Mono_25, 4, 77+19);
			break;
	}
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
	display.print_string((unsigned char *) "temp0", Roboto_Mono_12, 3, 45);
	//////////////// End Splash Screen Animation.

	// Run heater while the splash screen is displayed.
	sensor.run_heater(5);
	display.clear_screen();

	// After all other initialisations are complete, enable the push-button interrupt.
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button, LOW);

	// Enable the timer interrupt (for LED pulsing).
	init_timer();

	// Turn off built-in LED to indicate Pro Trinket has finished "booting".
	digitalWrite(LED_BUILTIN, LOW);
}

void loop(void)
{
	double *sensor_array = sensor.get_sensor_data();		// Update temperature and humidity readings.

	send_data(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Send temperature and humidity readings to the esp8266 (converts double to string).

	update_oled(sensor_array[TEMPERATURE], sensor_array[HUMIDITY]);	// Update the oled display with the latest  temperature and humidity readings.
}

