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
	page += "	text-align: center;\n";
	page += "	background-color: rgb(50,50,50);\n";
	page += "	color: rgb(150,150,150);\n";
	page += "	font-weight: bold; }\n";
	page += "h1 {	font-size: 96px; }\n";
	page += "h2 {	height: 50px;\n";
	page += "	margin: 40px 0px 0px 0px;\n";
	page += "	font-size: 48px; }\n";
	page += "h3 {	height: 50px;\n";
	page += "	color: black;\n";
	page += "	margin: 0px 0px 0px 0px;\n";
	page += "	font-size: 48px; }\n";
	page += "</style>\n";
	page += "</head>\n";
	page += "<body>\n";
	page += "<h1>temp0</h1>\n";
	page += "<h2>Temperature</h2>\n";
	page += "<h3>" + get_temp() + "&deg;C</h3>\n";
	page += "<h2>Humidity</h2>\n";
	page += "<h3>" + get_humi() + "%</h3>\n";
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

	server.onNotFound(handle_not_found);

	server.begin();
	Serial.println("HTTP server started");
}

void loop(void) {
	server.handleClient();
	MDNS.update();
}