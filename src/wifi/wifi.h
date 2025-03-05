// src/wifi/wifi.h
#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "fake_ap/fake_ap.h"
#include "evil_twin/clone_ap/clone_ap.h"

// External variables (defined in main sketch) that wifi code might need to access
extern Adafruit_SSD1306 display;
extern bool inWiFiMenu;
extern uint8_t wifiIndex;
extern uint8_t selectedIndex;
extern uint8_t settingIndex;
extern bool inSettings;
extern bool inFakeAPMenu;
extern uint8_t fakeAPIndex;

// Global Variables - DECLARATIONS using 'extern' in wifi.h
extern bool inCloneAPMenu;
extern uint8_t cloneAPIndex;
extern std::vector<WiFiNetwork> scannedNetworks;
extern bool inEvilTwinMenu;
extern uint8_t evilTwinIndex;


// WiFi and AP related variables
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern char apSSID[50];;
extern const char *apPassword;
extern const char *captivePortalPage;
extern bool fakeAPEnabled;

void handleClientRequest();

#endif // WIFI_H