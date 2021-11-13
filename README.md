# temp0
A temperature/humidity sensor that provides live readings over WiFi and via an oled display.
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

The code was developed using the [PlatformIO][link_web_platformio] extension in [VSCode][link_web_vscode].  The hardware was designed using [KiCAD][link_web_kicad] and fabricated using [JLCPCB][link_web_jlcpcb].

All components are on a custom PCB and inside an enclosure with a transperant lid so as to show off the special-edition [HackADay 10-year anniversary Pro Trinket][link_web_hackaday_pro_trinket]!

More info can be found [here][link_clews_projects_temp0].

## Goals
This project was developed to meet a few goals:
- Create a device for monitoring internal temperature - to be compatible with automation platforms (specifically [Home Assistant][link_web_home_assistant]).
	- Local temperature read-out.
	- Simple web-server for obtaining temperature over WiFi.
	- Simple text-only response when calling a specific url with curl (also over WiFi). 
- Make use of - and potentially showcase - a special [HackADay 10-year anniversary edition Pro-Trinket][link_web_hackaday_pro_trinket] dev board.
- Learn how to use an [esp8266][link_web_esp8266].
- Try out [arduino][link_web_arduino] (instead of directly programming microcontrollers).
	- Try the [arduino IDE][link_web_arduino_ide] and programming via the arduino bootloader.
	- Try using some [C++][link_wiki_c++] for development instead of just [C][link_wiki_c].

## Components
The final system consists of the following components:
- [Pro Trinket 5V][link_repo_data_pro_trinket] (Hackaday 10-Year Anniversary Special-Edition) - Controls the SSD1306 and the HDC1080. Sends temperature/humidity readings over serial to the ESP8266 module.
- [ESP8266][link_repo_data_esp8266] (ESP-01 module) - Acts as a web-server to provide temperature/humidity over WiFi.
- [HDC1080][link_repo_data_hdc1080] module - Temperature/Humidity sensor that communicates over I2C.
- [SSD1306][link_repo_data_ssd1306] OLED module - also communicates over I2C - used as a local display for temperature/humidity readings.
- Miscellaneous components for power conversion (5V->3.3V), filtering, IO pull-ups, etc.

## Repository
This repo contains:
- [data][link_repo_temp0_data] - datasheets I referenced to write relevant code and create custom [KiCAD][link_web_kicad] footprints.
- [images][link_repo_temp0_images] - photos of the final project.
- [temp0_esp8266][link_repo_temp0_esp8266] - code written for the ESP8266/ESP-01 module.
- [temp0_pro_trinket][link_repo_temp0_pro_trinket] - code written for the Pro Trinket module.
- [temp0_schematic][link_repo_temp0_schematic] - [KiCAD][link_web_kicad] files including the schematic and PCB layout.  Library includes some custom parts (ESP-01, Pro-Trinket, HDC1080, OLED).
- [temp0_workspace.code-workspace][link_repo_temp0_workspace] - Workspace structure for VSCode including PlatformIO configurations for working with the ESP8266/ESP-01 and the Pro-Trinket 5V.

## Gallery
### Start-up animation.
![Demo of startup][image_temp0_demo_startup]

### Cycling through different display modes.
![Demo of display modes][image_temp0_demo_modes]

### Assembled PCB.
![Assembled unit][image_temp0_assembled]

### In an enclosure.
![Buttoned up][image_temp0_complete]

### Web interface.
![WebUI][image_temp0_webui]


[link_clews_projects_temp0]:https://clews.pro/projects/temp0.php

[link_repo_temp0_data]:/data
[link_repo_data_esp8266]:/data/esp8266_pinout.png
[link_repo_data_hdc1080]:/data/hdc1080_i2c_temperature_humidity_sensor.pdf
[link_repo_data_pro_trinket]:/data/pro_trinket_5V_pinout.jpeg
[link_repo_data_ssd1306]:/data/ssd1306_oled_driver.pdf

[link_repo_temp0_esp8266]:/temp0_esp8266
[link_repo_temp0_images]:/images
[link_repo_temp0_pro_trinket]:/temp0_pro_trinket
[link_repo_temp0_schematic]:/temp0_schematic
[link_repo_temp0_workspace]:/temp0_workspace.code-workspace

[link_web_arduino]:https://www.arduino.cc/
[link_web_arduino_ide]:https://www.arduino.cc/en/Main/Software
[link_web_esp8266]:http://esp8266.net/
[link_web_hackaday_pro_trinket]:https://hackaday.com/2014/10/10/10th-anniversary-trinket-pro-now-in-the-hackaday-store/
[link_web_home_assistant]:https://www.home-assistant.io/
[link_web_jlcpcb]:https://jlcpcb.com/
[link_web_kicad]:https://kicad.org/
[link_web_platformio]:https://platformio.org/
[link_web_vscode]:https://code.visualstudio.com/

[link_wiki_c]:https://en.wikipedia.org/wiki/C_(programming_language)
[link_wiki_c++]:https://en.wikipedia.org/wiki/C%2B%2B

[image_temp0_assembled]:/images/temp0_assembled.jpg
[image_temp0_complete]:/images/temp0_complete.jpg
[image_temp0_demo_modes]:/images/temp0_demo_modes.gif
[image_temp0_demo_startup]:/images/temp0_demo_startup.gif
[image_temp0_webui]:/images/temp0_webui.png
