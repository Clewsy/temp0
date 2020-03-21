#include <Arduino.h>

#include "hdc1080.h"
#include "ssd1306.h"

//Global variables
hdc1080 sensor;		//Initialise an hdc1080 sensor called "sensor".  Refer "hdc1080.h".
ssd1306 display;	//Initialise an oled display with ssd1306 driver called "display".  Refer "ssd1306.h"