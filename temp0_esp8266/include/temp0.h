// Include header files for required esp8266 libraries.
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// WiFi credentials (ESSID and password) can be stored here or otherwise in an external header file "credentials.h".
#include "credentials.h"
#ifndef STASSID
#	define STASSID	"your-ssid"
#	define STAPSK	"your-password"
#endif

// Define the hostname used by the esp8266.
#define HOSTNAME "temp0"
