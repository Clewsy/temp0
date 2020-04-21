# temp0 - THIS PROJECT IS IN DEVELOPMENT
A temperature/humidity sensor that provides live readings on an oled display and also on a web page served over WiFi.
Temperature and humidity readouts are displayed directly on an OLED screen.
The device connects to a local network over WiFi and serves the following:
- Pretty html page at root:
	- http://temp0
- Simple text only temperature value at:
	- http://temp0/temperature
	- http://temp0/temp
	- http://temp0/t
- Simple text only humidity value at:
	- http://temp0/humidity
	- http://temp0/humi
	- http://temp0/h

I made a custom PCB and put everything in an enclosure with a transperant lid so as to show off the special Pro Trinket I've had sitting in a drawer for six years!

More info can be found [here][link_clews_projects_temp0].

## Goals
This project was developed to meet a few goals:
- Create a device for monitoring internal temperature - to be compatible with automation platforms (specifically [HassIO][link_web_hassio]).
	- Local temperature read-out.
	- Simple web-server for obtaining temperature over WiFi.
	- Simple text-only response when calling a specific url with curl (also over WiFi). 
- Make use of - and potentially showcase - a special [HackADay 10-year anniversary edition Pro-Trinket][link_web_hackaday_pro_trinket] dev board.
- Learn how to use an esp8266.
- Try out arduino (instead of directly programming microcontrollers).
	- Try the arduino IDE and programming via the arduino bootloader.
	- Try using some C++ for development instead of just C.

## Components
The final system consists of the following components:
- Pro Trinket 5V (Hackaday 10-Year Anniversary Special-Edition) - Controls the SSD1306 and the HDC1080. Sends temperature/humidity readings over serial to the ESP8266 module.
- ESP8266 (ESP-01 module) - Acts as a web-server to provide temperature/humidity over WiFi.
- HDC1080 module - Temperature/Humidity sensor that communicates over I2C.
- SSD1306 OLED module - also communicates over I2C - used as a local display for temperature/humidity readings.
- Miscellaneous components for power conversion (5V->3.3V), filtering, IO pull-ups, etc.

[link_web_hassio]:https://www.home-assistant.io/hassio/
[link_web_hackaday_pro_trinket]:https://hackaday.com/2014/10/10/10th-anniversary-trinket-pro-now-in-the-hackaday-store/
[link_clews_projects_temp0]:https://clews.pro/projects/temp0.php
