#include <Arduino.h>

#include "hdc1080.h"	// Define the class and functions for operating an HDC1080 Temperature/Humidity sensor.
#include "ssd1306.h"	// Define the class and functions for operating an SSD1306 OLED driver.

// Define values for various oled display modes.
#define MODE_NORMAL		0	// 0b000
#define MODE_NORMAL_INVERSE	1	// 0b001
#define MODE_LARGE		2	// 0b010
#define MODE_LARGE_INVERSE	3	// 0b011
#define MODE_TEMP_ONLY		4	// 0b100
#define MODE_TEMP_ONLY_INVERSE	5	// 0b101
#define MODE_LOGO		6	// 0b110
#define MODE_LOGO_INVERSE	7	// 0b111

#define BUTTON_PIN	3	// Allocate push-button hardware input pin.  For Pro-Trinket, pin 3 is the only option for hardware interrupt capability.
#define BUTTON_DEBOUNCE	20	// Duration in milliseconds to register a press of the button (debounce timer).

#define LED_EXTERNAL	6	// LED connected to this pin.  On Pro-Trinket, this pin has analog out capability.
#define LED_PULSE_SPEED	16	// Increment size for increasing/decreasing led brightness.
#define GOING_UP	0	// LED pulse direction, brigtness increasing.
#define GOING_DOWN	1	// LED pulse direction, brigtness decreasing.

// Global variables.
hdc1080 sensor;			// Initialise an hdc1080 sensor called "sensor".  Refer "hdc1080.h".
ssd1306 display;		// Initialise an oled display with ssd1306 driver called "display".  Refer "ssd1306.h"

uint8_t mode = MODE_NORMAL;		// Initialise the "start-up" mode.
uint8_t last_mode = MODE_NORMAL;	// Initialise "last_mode" - needed to determine if mode has changed.
long trigger_time = 0;			// Initialise the timer comparator for debouncing a button press.
uint8_t led_value = 0;			// Initialise the brightnes value of the external led.
uint8_t led_pulse_dir = GOING_UP;	// Initialise direction of led pulse.
