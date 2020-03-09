#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "credentials.h"

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

String get_temp() {
	Serial.find("t");
	String temp = Serial.readStringUntil(';');
	if(temp == "") {
		temp = "error";
	}
	return temp;
}

String get_humi() {
	Serial.find("h");
	String humi = Serial.readStringUntil(';');
	if(humi == "") {
		humi = "error";
	}
	return humi;
}

void handle_temp() {
	server.send(200, "text/plain", get_temp());
}

void handle_humi() {
	server.send(200, "text/plain", get_humi());
}


//void handle_favicon() {
//	server.send(200, "text/html", "<link rel='icon' href='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAS0lEQVR42s2SMQ4AIAjE+P+ncSYdasgNXMJgcyIIlVKPIKdvioAXyWBeJmVpqRZKWtj9QWAKZyWll50b8IcL9JUeQF50n28ckyb0ADG8RLwp05YBAAAAAElFTkSuQmCC' type='image/x-png' />");
//}

void handle_root() {
	String page = "<!DOCTYPE html>\n"; 
	page += "<html>\n";
	page += "<head>\n";
	page += "<link rel='icon' href='data:image/png;base64,";
	page += "iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAABmJLR0QA/wD/AP+gvaeTAAACqklE";
	page += "QVR4nO2byW4TQRCGv3hJciACBDeWO1JuPAKJWAQigsADkCMJm1+AIwHlLUC8CViRgxMgGClckkhw";
	page += "BBFAYjMcqlpjWV5i1FOFrf6kVqtrqjV//Z7pGdszkEhYMwHcAqrAF21VYEm3jTTHgXXgT5e2rjkj";
	page += "ySSwgRS6A1wFprTNAW/JTBjJI+E2UmADONxh+yEyE5YMdZmxihR3uUfOFc2pmigyZg8pbqpHzkHN";
	page += "+WyiyJiw0MXKi0LBakf/K8kAbwHeJAO8BXjjacB5YFfbBUcdZrRf3nZbYjs98nJlzGpHZEWNtY3b";
	page += "tbTn5UpaA7wFeJMM8BbgTTLAW4A3yQBvAd4kA7wFeJMM8BbgTTLAW4A3yQBvAd4kA7wFeJMM8Bbg";
	page += "TTLAW4A3yQBvAd4kA7wFeJMMMNrP/X+Y8yC2CC9WkD88fwCVlnj7U6KBCvBTYytGGnPjHNBEip/R";
	page += "WAlY0HgovqmxkubMIiY0gbOGeqNSBraRAu9q7Cjwgu7PCq8BRzS3orFtMmOGiktIAVvIWlNCCuxW";
	page += "fGg1zS0A7zR20Vh7FJ4i4u/oeIH+xYd2Q+fc0/ETM9UReYmIP63jGvs3YFXnnNLxhpnqiHxCxIcn";
	page += "w7+xfwO+6pxJHX/MS2Se9wFF7X9p/3uAuWHOuPa56czTgPfan9D+zQBzQ+5J7T9EUdSBPA1Y035W";
	page += "+8cDzA2LXphbi6LImOvI+fsKOR0mgDr9z/86cugXgU2NzRtrj8I42XX8psaO0duEuuaAvFkW7iPK";
	page += "ZqojE16B+Q6c0dg4sAg8R94i2QOeaSwsejPI7XO/V2yGgkdkJiySXR06UUQ++VD8cu7qDCgAD8kO";
	page += "89fId4Np4IC2aeSub7Mlb5kR+71iDjmf+y2CW4zAYd+NMnANuSQ2yNaAhsbmGeIFLzGM/AVYSNm1";
	page += "OH1mOwAAAABJRU5ErkJggg==' ";
	page += "type='image/x-png' />\n";
	page += "<title>temp0</title>\n";
	page += "<style>\n";
	page += "body {	background-color: rgb(50,50,50);\n"; 
	page += "	font-family: monospace;\n";
	page += "	font-size: 48px;\n";
	page += "	text-align: center;\n";
	page += "	background-color: rgb(50,50,50);\n";
	page += "	font-weight: bold; }\n";
	page += "h1 {	color: rgb(150,150,150);\n";
	page += "	font-size: 96px; }\n";
	page += "</style>\n";
	page += "</head>\n";
	page += "<body>\n";
	page += "<h1>temp0</h1>\n";
	page += "<p>Temperature: ";
	page += get_temp();
	page += "&deg;C</p>\n";
	page += "<p>Humidity: ";
	page += get_humi();
	page += "%</p>\n";
	page += "</body>\n";
	page += "</html>\n";

	server.send(200, "text/html", page);
}


void handle_not_found() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
}

void setup(void) {
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

	if (MDNS.begin("esp8266")) {
		Serial.println("MDNS responder started");
	}

	server.on("/", handle_root);

	server.on("/t", handle_temp);
	server.on("/temp", handle_temp);
	server.on("/temperature", handle_temp);

	server.on("/h", handle_humi);
	server.on("/humi", handle_humi);
	server.on("/humidity", handle_humi);

//	server.on("/favicon.ico", handle_favicon);

	server.on("/gif", []() {
		static const uint8_t gif[] PROGMEM = {
			0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
			0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
			0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
			0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
			0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
		};
		char gif_colored[sizeof(gif)];
		memcpy_P(gif_colored, gif, sizeof(gif));
		// Set the background to a random set of colors
		gif_colored[16] = millis() % 256;
		gif_colored[17] = millis() % 256;
		gif_colored[18] = millis() % 256;
		server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
	});

	server.onNotFound(handle_not_found);

	server.begin();
	Serial.println("HTTP server started");
}

void loop(void) {
	server.handleClient();
	MDNS.update();
}