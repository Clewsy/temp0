#include <Arduino.h>

#include "hdc1080.h"	// Define the class and functions for operating an HDC1080 Temperature/Humidity sensor.
#include "ssd1306.h"	// Define the class and functions for operating an SSD1306 OLED driver.

// Define values for various oled display modes.  Note, lsb can be used to determine inverse.
#define MODE_LARGE		0	// 0b0000
#define MODE_LARGE_INVERSE	1	// 0b0001
#define MODE_TEMP_ONLY		2	// 0b0010
#define MODE_TEMP_ONLY_INVERSE	3	// 0b0011
#define MODE_NORMAL		4	// 0b0100
#define MODE_NORMAL_INVERSE	5	// 0b0101
#define MODE_BLANK		6	// 0b0110

#define BUTTON_PIN	3	// Allocate push-button hardware input pin.  For Pro-Trinket, pin 3 is the only option for hardware interrupt capability.
#define BUTTON_DEBOUNCE	20	// Duration in milliseconds to register a press of the button (debounce timer).

#define LED_EXTERNAL	6	// LED connected to this pin.  On Pro-Trinket, this pin has analog out capability.
#define LED_PULSE_SPEED	1	// Increment size for increasing/decreasing led brightness.
#define LED_MAX_VALUE	100	// Maximum brightness value of external LED (analogue out 0 to 255).

// Function declarations.
uint8_t mode = MODE_LARGE;	// Initialise the "start-up" mode.
uint8_t last_mode = mode;	// Initialise "last_mode" - needed to determine if mode has changed.
long trigger_time = 0;		// Initialise the timer comparator for debouncing a button press.
uint8_t led_value = 0;		// Initialise the brightnes value of the external led.
int8_t led_pulse_dir = 1;	// Initialise direction of led pulse.  1 for brightness increasing, -1 for brightness decreasing.
