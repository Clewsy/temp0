#include <Arduino.h>

#include "hdc1080.h"	//Define the class and functions for operating an HDC1080 Temperature/Humidity sensor.
#include "ssd1306.h"	//Define the class and functions for operating an SSD1306 OLED driver.

//Define values for various modes.
#define MODE_NORMAL	0
#define MODE_WEIRD	1

#define BUTTON_PIN	3	//Allocate push-button hardware input pin.  For Pro-Trinket, pin 3 is the only option for hardware interrupt capability.
#define BUTTON_DEBOUNCE	20	//Duration in milliseconds to register a press of the button.

//Global variables
hdc1080 sensor;			//Initialise an hdc1080 sensor called "sensor".  Refer "hdc1080.h".
ssd1306 display;		//Initialise an oled display with ssd1306 driver called "display".  Refer "ssd1306.h"

uint8_t mode = MODE_NORMAL;	//Initialise the "start-up" mode.
long trigger_time = 0;		//Initialise the timer comparator for debouncing a button press.
