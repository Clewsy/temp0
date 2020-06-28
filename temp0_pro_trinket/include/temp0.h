#include <Arduino.h>

#include "hdc1080.h"	// Define the class and functions for operating an HDC1080 Temperature/Humidity sensor.
#include "ssd1306.h"	// Define the class and functions for operating an SSD1306 OLED driver.

// Define values for various oled display modes.
#define MODE_LARGE		0	// 0b0000
#define MODE_LARGE_INVERSE	1	// 0b0001
#define MODE_TEMP_ONLY		2	// 0b0010
#define MODE_TEMP_ONLY_INVERSE	3	// 0b0011
#define MODE_NORMAL		4	// 0b0100
#define MODE_NORMAL_INVERSE	5	// 0b0101
#define MODE_LOGO_CLEWS		6	// 0b0110
#define MODE_LOGO_CLEWS_INVERSE	7	// 0b0111
#define MODE_LOGO_HAD		8	// 0b1000
#define MODE_LOGO_HAD_INVERSE	9	// 0b1001
#define MODE_BLANK		10	// 0b1010

#define BUTTON_PIN	3	// Allocate push-button hardware input pin.  For Pro-Trinket, pin 3 is the only option for hardware interrupt capability.
#define BUTTON_DEBOUNCE	20	// Duration in milliseconds to register a press of the button (debounce timer).

#define LED_EXTERNAL	6	// LED connected to this pin.  On Pro-Trinket, this pin has analog out capability.
#define LED_PULSE_SPEED	1	// Increment size for increasing/decreasing led brightness.
#define LED_MAX_VALUE	20	// Maximum brightness value of external LED (analogue out 0 to 255).

// Global variables.
hdc1080 sensor;			// Initialise an hdc1080 sensor called "sensor".  Refer "hdc1080.h".
ssd1306 display;		// Initialise an oled display with ssd1306 driver called "display".  Refer "ssd1306.h"

uint8_t mode = MODE_LARGE;	// Initialise the "start-up" mode.
uint8_t last_mode = mode;	// Initialise "last_mode" - needed to determine if mode has changed.
long trigger_time = 0;		// Initialise the timer comparator for debouncing a button press.
uint8_t led_value = 0;		// Initialise the brightnes value of the external led.
int8_t led_pulse_dir = 1;	// Initialise direction of led pulse.  1 for brightness increasing, -1 for brightness decreasing.
