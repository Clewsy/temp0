#include <Arduino.h>

#include "hdc1080.h"	// Define the class and functions for operating an HDC1080 Temperature/Humidity sensor.
#include "ssd1306.h"	// Define the class and functions for operating an SSD1306 OLED driver.

// Define values for various oled display modes.
#define MODE_NORMAL		0
#define MODE_LARGE		1
#define MODE_NORMAL_INVERSE	2
#define MODE_LARGE_INVERSE	3

#define BUTTON_PIN	3	// Allocate push-button hardware input pin.  For Pro-Trinket, pin 3 is the only option for hardware interrupt capability.
#define BUTTON_DEBOUNCE	20	// Duration in milliseconds to register a press of the button.

#define LED_EXTERNAL	6	// LED connected to this pin.  On pro trinket, this pin has analog out capability.
#define LED_PULSE_SPEED	16	// Increment size for increasing/decreasing led brightness.
#define GOING_UP	0	// LED pulse direction, brigtness increasing.
#define GOING_DOWN	1	// LED pulse direction, brigtness decreasing.

// Global variables
hdc1080 sensor;			// Initialise an hdc1080 sensor called "sensor".  Refer "hdc1080.h".
ssd1306 display;		// Initialise an oled display with ssd1306 driver called "display".  Refer "ssd1306.h"

uint8_t mode = MODE_NORMAL;		// Initialise the "start-up" mode.
uint8_t last_mode = MODE_NORMAL;	// Initialise "last_mode" - needed to determine if mode has changed.
long trigger_time = 0;			// Initialise the timer comparator for debouncing a button press.
uint8_t led_value = 0;			// Initialise the brightnes value of the external led.
uint8_t led_pulse_dir = GOING_UP;	// Initialise direction of led pulse.
